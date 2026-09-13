#!/usr/bin/env python3
"""Compile glTF/GLB source assets into Itajai Drive's compact runtime PAK.

Supported source subset is intentionally small but standard: one mesh primitive,
POSITION accessor required, optional indexed triangles, embedded/external buffers,
and .glb JSON/BIN chunks. Runtime vertices are baked as pos3/normal3/uv2 floats.
Assets may opt into position-welded normal groups with extras.smoothNormals=true;
this keeps procedural vehicle shells visually smooth without changing runtime cost.
"""
import argparse, base64, json, math, pathlib, struct, urllib.parse

MAGIC=b'IDAPAK08'
VERSION=1
REC=struct.Struct('<32sIIf3f3fQQ')
VERT=struct.Struct('<8f')

def read_source(path: pathlib.Path):
    raw=path.read_bytes()
    glb_bin=None
    if path.suffix.lower()=='.glb':
        if len(raw)<20 or raw[:4]!=b'glTF': raise ValueError(f'{path}: invalid GLB')
        magic,ver,total=struct.unpack_from('<III',raw,0)
        if ver!=2 or total>len(raw): raise ValueError(f'{path}: unsupported GLB')
        off=12; doc=None
        while off+8<=total:
            ln,typ=struct.unpack_from('<II',raw,off); off+=8
            chunk=raw[off:off+ln]; off+=ln
            if typ==0x4E4F534A: doc=json.loads(chunk.rstrip(b'\0 \t\r\n').decode('utf-8'))
            elif typ==0x004E4942: glb_bin=chunk
        if doc is None: raise ValueError(f'{path}: GLB has no JSON chunk')
    else:
        doc=json.loads(raw.decode('utf-8'))
    return doc,glb_bin

def load_buffer(doc, glb_bin, base: pathlib.Path, index=0):
    b=doc['buffers'][index]
    uri=b.get('uri')
    if uri is None:
        if glb_bin is None: raise ValueError('buffer has no URI/BIN chunk')
        return glb_bin
    if uri.startswith('data:'):
        head,data=uri.split(',',1)
        return base64.b64decode(data) if ';base64' in head else urllib.parse.unquote_to_bytes(data)
    return (base/urllib.parse.unquote(uri)).read_bytes()

def accessor(doc,buffers,idx):
    a=doc['accessors'][idx]; v=doc['bufferViews'][a['bufferView']]
    buf=buffers[v.get('buffer',0)]
    ctype=a['componentType']; typ=a['type']; count=a['count']
    ncomp={'SCALAR':1,'VEC2':2,'VEC3':3,'VEC4':4}[typ]
    fmt={5121:'B',5123:'H',5125:'I',5126:'f'}[ctype]
    comp=struct.calcsize('<'+fmt); stride=v.get('byteStride',comp*ncomp)
    base=v.get('byteOffset',0)+a.get('byteOffset',0)
    out=[]
    for i in range(count):
        out.append(struct.unpack_from('<'+fmt*ncomp,buf,base+i*stride))
    return out

def normalize(v):
    x,y,z=v; l=math.sqrt(x*x+y*y+z*z)
    return (x/l,y/l,z/l) if l>1e-12 else (0.0,1.0,0.0)

def smooth_normal_groups(pos,norms,precision=5):
    """Average normals for duplicate shell vertices that share a position."""
    groups={}
    for p,n in zip(pos,norms):
        k=(round(p[0],precision),round(p[1],precision),round(p[2],precision))
        a=groups.setdefault(k,[0.0,0.0,0.0]);a[0]+=n[0];a[1]+=n[1];a[2]+=n[2]
    groups={k:normalize(v) for k,v in groups.items()}
    return [groups[(round(p[0],precision),round(p[1],precision),round(p[2],precision))] for p in pos]

def compile_one(path: pathlib.Path):
    doc,glb_bin=read_source(path)
    buffers=[load_buffer(doc,glb_bin,path.parent,i) for i in range(len(doc.get('buffers',[])))]
    prim=doc['meshes'][0]['primitives'][0]
    if prim.get('mode',4)!=4: raise ValueError(f'{path}: only TRIANGLES mode is supported')
    pos=[tuple(map(float,p[:3])) for p in accessor(doc,buffers,prim['attributes']['POSITION'])]
    if 'indices' in prim: inds=[int(x[0]) for x in accessor(doc,buffers,prim['indices'])]
    else: inds=list(range(len(pos)))
    if len(inds)%3: raise ValueError(f'{path}: index count is not divisible by 3')
    norms=[[0.0,0.0,0.0] for _ in pos]
    for i in range(0,len(inds),3):
        ia,ib,ic=inds[i:i+3]; a,b,c=pos[ia],pos[ib],pos[ic]
        ab=(b[0]-a[0],b[1]-a[1],b[2]-a[2]); ac=(c[0]-a[0],c[1]-a[1],c[2]-a[2])
        n=(ab[1]*ac[2]-ab[2]*ac[1],ab[2]*ac[0]-ab[0]*ac[2],ab[0]*ac[1]-ab[1]*ac[0])
        for k in (ia,ib,ic): norms[k][0]+=n[0]; norms[k][1]+=n[1]; norms[k][2]+=n[2]
    if doc.get('extras',{}).get('smoothNormals',False): norms=smooth_normal_groups(pos,norms)
    else: norms=[normalize(n) for n in norms]
    mins=[min(p[j] for p in pos) for j in range(3)]; maxs=[max(p[j] for p in pos) for j in range(3)]
    sx=max(maxs[0]-mins[0],1e-6); sz=max(maxs[2]-mins[2],1e-6)
    verts=[(*p,*n,(p[0]-mins[0])/sx,(p[2]-mins[2])/sz) for p,n in zip(pos,norms)]
    collider=float(doc.get('extras',{}).get('itajaiColliderRadius',max(sx,sz)*.5))
    name=doc.get('meshes',[{}])[0].get('name',path.stem)
    return name,verts,inds,collider,mins,maxs

def main():
    ap=argparse.ArgumentParser(); ap.add_argument('-o','--output',required=True); ap.add_argument('assets',nargs='+'); a=ap.parse_args()
    compiled=[compile_one(pathlib.Path(x)) for x in a.assets]
    header_size=8+4+4+REC.size*len(compiled); cursor=header_size; records=[]; payload=[]
    for name,verts,inds,collider,mins,maxs in compiled:
        vb=b''.join(VERT.pack(*v) for v in verts); ib=b''.join(struct.pack('<I',i) for i in inds)
        vo=cursor; cursor+=len(vb); io=cursor; cursor+=len(ib)
        records.append(REC.pack(name.encode('utf-8')[:31].ljust(32,b'\0'),len(verts),len(inds),collider,*mins,*maxs,vo,io)); payload += [vb,ib]
    out=pathlib.Path(a.output); out.parent.mkdir(parents=True,exist_ok=True)
    with out.open('wb') as f:
        f.write(MAGIC); f.write(struct.pack('<II',VERSION,len(compiled)))
        for r in records: f.write(r)
        for p in payload: f.write(p)
    print(f'{out}: {len(compiled)} assets, {out.stat().st_size} bytes')

if __name__=='__main__': main()
