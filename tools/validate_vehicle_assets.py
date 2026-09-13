#!/usr/bin/env python3
"""Fail CI if Renderer 5.0 vehicle generation regresses to cloned silhouettes."""
import base64, hashlib, json, pathlib, struct
from generate_vehicle_gltf import PRESETS

ROOT=pathlib.Path('assets/generated')


def positions(doc):
    acc=doc['accessors'][0]; view=doc['bufferViews'][acc['bufferView']]
    uri=doc['buffers'][view.get('buffer',0)]['uri']; raw=base64.b64decode(uri.split(',',1)[1])
    base=view.get('byteOffset',0)+acc.get('byteOffset',0); stride=view.get('byteStride',12)
    return [struct.unpack_from('<3f',raw,base+i*stride) for i in range(acc['count'])]


def normalized_signature(pos):
    lo=[min(p[k] for p in pos) for k in range(3)]; hi=[max(p[k] for p in pos) for k in range(3)]
    span=[max(1e-6,hi[k]-lo[k]) for k in range(3)]
    # Preserve generator vertex order while removing overall width/height/length.
    q=[]
    for p in pos:
        q.extend(round((p[k]-lo[k])/span[k],3) for k in range(3))
    return hashlib.sha256(repr(q).encode()).hexdigest(),lo,hi


def main():
    signatures={}; rows=[]
    for name,preset in PRESETS.items():
        path=ROOT/(name+'.gltf')
        if not path.exists(): raise SystemExit(f'missing generated body: {path}')
        doc=json.loads(path.read_text(encoding='utf-8')); extra=doc.get('extras',{})
        mesh=doc.get('meshes',[{}])[0].get('name','')
        if mesh!='car_'+name: raise SystemExit(f'{name}: unexpected mesh name {mesh!r}')
        if int(extra.get('silhouetteRevision',0))<51: raise SystemExit(f'{name}: silhouette revision < 51')
        if not extra.get('smoothNormals',False): raise SystemExit(f'{name}: HQ body lost smoothNormals contract')
        pos=positions(doc); inds=doc['accessors'][1]['count']
        if len(pos)<450 or inds<2400: raise SystemExit(f'{name}: suspiciously simple HQ body ({len(pos)} verts/{inds} indices)')
        sig,lo,hi=normalized_signature(pos)
        if sig in signatures: raise SystemExit(f'{name}: normalized silhouette duplicates {signatures[sig]}')
        signatures[sig]=name
        dims=(hi[0]-lo[0],hi[1]-lo[1],hi[2]-lo[2])
        # Preset H is ground-to-roof; generated paint shell starts ~26 cm above ground.
        body_h=max(.5,preset['h']-.26)
        if not (.88*preset['w']<=dims[0]<=1.08*preset['w']): raise SystemExit(f'{name}: body width drift {dims[0]:.3f}')
        if not (.86*body_h<=dims[1]<=1.14*body_h): raise SystemExit(f'{name}: body shell height drift {dims[1]:.3f}')
        if not (.94*preset['l']<=dims[2]<=1.04*preset['l']): raise SystemExit(f'{name}: body length drift {dims[2]:.3f}')
        rows.append((name,len(pos),inds,dims,sig[:10]))
    if len(signatures)!=len(PRESETS): raise SystemExit('vehicle silhouette uniqueness gate failed')
    print('Renderer 5.0 vehicle silhouette gate: PASS')
    for name,nv,ni,d,s in rows: print(f'  {name:24s} {nv:4d} verts {ni//3:4d} tris  {d[0]:.2f}x{d[1]:.2f}x{d[2]:.2f}  shape={s}')

if __name__=='__main__': main()
