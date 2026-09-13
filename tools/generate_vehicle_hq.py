#!/usr/bin/env python3
"""Renderer 5.0 vehicle body generator.

Builds smoother original procedural body shells from the existing Brazilian-market
proportion presets. The source proportions remain model-specific, but the old
faceted four-corner cross-section is replaced by interpolated rounded rings with
shared vertices so the runtime normal baker produces continuous highlights.
"""
import base64, json, math, pathlib, struct
from generate_vehicle_gltf import PRESETS, box


def sgn(v):
    return -1.0 if v < 0.0 else 1.0


def resample(sections, count):
    if len(sections) < 2 or count <= len(sections):
        return list(sections)
    out=[]
    for k in range(count):
        u=k*(len(sections)-1)/(count-1)
        i=min(len(sections)-2,int(u)); f=u-i
        a,b=sections[i],sections[i+1]
        out.append(tuple(a[j]+(b[j]-a[j])*f for j in range(4)))
    return out


def rounded_shell(v, ind, sections, sides=16, squash=.68):
    """Longitudinal rounded superellipse rings with shared vertices."""
    base=len(v)
    for z,w,b,t in sections:
        cy=(b+t)*.5; ry=(t-b)*.5
        for k in range(sides):
            a=2*math.pi*k/sides
            ca,sa=math.cos(a),math.sin(a)
            x=w*sgn(ca)*(abs(ca)**.58)
            y=cy+ry*sgn(sa)*(abs(sa)**squash)
            v.append((x,y,z))
    rows=len(sections)
    for r in range(rows-1):
        a0=base+r*sides; b0=base+(r+1)*sides
        for k in range(sides):
            n=(k+1)%sides
            ind += [a0+k,b0+k,b0+n, a0+k,b0+n,a0+n]
    # End caps as triangle fans. Reuse perimeter vertices so edge normals remain soft.
    for row,flip in ((0,True),(rows-1,False)):
        z,w,b,t=sections[row]; center=len(v);v.append((0,(b+t)*.5,z));r0=base+row*sides
        for k in range(sides):
            n=(k+1)%sides
            if flip: ind += [center,r0+n,r0+k]
            else: ind += [center,r0+k,r0+n]


def add_character(v,ind,p):
    W,L=p['w'],p['l']; st=p['style']
    # Crisp secondary forms on top of the smooth primary body keep each model readable.
    if st=='pickup':
        box(v,ind,0,.50,-L*.315,W*.90,.10,L*.35)
        box(v,ind,-W*.455,.76,-L*.315,.10,.48,L*.35);box(v,ind,W*.455,.76,-L*.315,.10,.48,L*.35)
        box(v,ind,0,.75,-L*.475,W*.92,.47,.10)
    elif st=='sedan':
        box(v,ind,0,.69,-L*.39,W*.82,.14,L*.18);box(v,ind,0,.61,L*.415,W*.78,.12,L*.13)
    elif st=='box_suv':
        box(v,ind,-W*.44,.47,0,.075,.20,L*.69);box(v,ind,W*.44,.47,0,.075,.20,L*.69)
        box(v,ind,0,.58,L*.48,W*.84,.34,.12)
    elif st=='box_hatch':
        box(v,ind,0,.54,-L*.475,W*.83,.30,.10);box(v,ind,0,.50,L*.475,W*.79,.25,.11)
    elif st=='modern_hatch':
        box(v,ind,0,.55,L*.46,W*.71,.09,.13);box(v,ind,0,.58,-L*.47,W*.78,.12,.10)
    else:
        box(v,ind,0,.56,L*.46,W*.75,.12,.11);box(v,ind,0,.58,-L*.46,W*.78,.14,.10)
    # Fender/shoulder volumes give reflected light something to catch around wheel arches.
    wb=p['wheelbase'];fz=wb*.5
    for z in (-fz,fz):
        box(v,ind,-W*.465,.49,z,.055,.22,.54)
        box(v,ind, W*.465,.49,z,.055,.22,.54)


def build(name,p):
    v=[];ind=[];W,L=p['w'],p['l']
    lower=[(zf*L,wf*W*.5,.26,top) for zf,wf,top in p['lower']]
    roof=[(zf*L,wf*W*.5,base,top) for zf,wf,base,top in p['roof']]
    lower=resample(lower,19);roof=resample(roof,13)
    rounded_shell(v,ind,lower,16,.66)
    rounded_shell(v,ind,roof,14,.73)
    add_character(v,ind,p)
    pos=b''.join(struct.pack('<3f',*q) for q in v)
    idx=b''.join(struct.pack('<H',q) for q in ind)
    raw=pos+idx
    mins=[min(q[a] for q in v) for a in range(3)];maxs=[max(q[a] for q in v) for a in range(3)]
    uri='data:application/octet-stream;base64,'+base64.b64encode(raw).decode()
    doc={
      'asset':{'version':'2.0','generator':'Itajai Drive Renderer 5.0 HQ vehicle generator'},
      'extras':{'itajaiColliderRadius':W*.62,'realWorldReference':name,'wheelbase':p['wheelbase'],'silhouetteRevision':50,'smoothBody':True},
      'buffers':[{'byteLength':len(raw),'uri':uri}],
      'bufferViews':[{'buffer':0,'byteOffset':0,'byteLength':len(pos),'target':34962},{'buffer':0,'byteOffset':len(pos),'byteLength':len(idx),'target':34963}],
      'accessors':[{'bufferView':0,'componentType':5126,'count':len(v),'type':'VEC3','min':mins,'max':maxs},{'bufferView':1,'componentType':5123,'count':len(ind),'type':'SCALAR'}],
      'meshes':[{'name':'car_'+name,'primitives':[{'attributes':{'POSITION':0},'indices':1,'mode':4}]}],
      'nodes':[{'mesh':0}],'scenes':[{'nodes':[0]}],'scene':0
    }
    return json.dumps(doc,separators=(',',':'))


def main():
    out=pathlib.Path('assets/generated');out.mkdir(parents=True,exist_ok=True)
    for name,p in PRESETS.items():
        path=out/(name+'.gltf');path.write_text(build(name,p),encoding='utf-8');print('HQ',path)

if __name__=='__main__':
    main()
