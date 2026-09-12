#!/usr/bin/env python3
"""Generate generic Brazil-inspired low-poly vehicle body glTF sources.

These are intentionally fictional silhouettes (no trademarks / copied manufacturer
meshes). Wheels and suspension are rendered separately by the native engine.
"""
import base64,json,pathlib,struct

PRESETS={
 'compact_hatch': dict(w=1.76,l=3.82,h=.62,cw=1.52,cl=1.92,ch=.70,cz=.08,hood=.90,rear=.56),
 'compact_sedan': dict(w=1.78,l=4.32,h=.60,cw=1.50,cl=1.86,ch=.68,cz=.05,hood=1.05,rear=.92),
 'sport_hatch':   dict(w=1.82,l=4.04,h=.56,cw=1.57,cl=1.88,ch=.62,cz=.03,hood=1.00,rear=.60),
 'compact_suv':   dict(w=1.86,l=4.28,h=.72,cw=1.62,cl=2.12,ch=.82,cz=.00,hood=.92,rear=.70),
 'city_hatch':    dict(w=1.67,l=3.58,h=.64,cw=1.43,cl=1.72,ch=.72,cz=.10,hood=.72,rear=.50),
 'small_pickup':  dict(w=1.79,l=4.46,h=.63,cw=1.49,cl=1.52,ch=.72,cz=.34,hood=.98,rear=1.30),
 'wagon':         dict(w=1.76,l=4.35,h=.64,cw=1.51,cl=2.50,ch=.70,cz=-.10,hood=.94,rear=.70),
 'midsize_sedan': dict(w=1.84,l=4.62,h=.61,cw=1.57,cl=2.02,ch=.69,cz=.02,hood=1.12,rear=1.02),
}

def box(verts,inds,cx,cy,cz,sx,sy,sz):
    base=len(verts);x=sx/2;y=sy/2;z=sz/2
    verts += [(cx-x,cy-y,cz-z),(cx+x,cy-y,cz-z),(cx+x,cy+y,cz-z),(cx-x,cy+y,cz-z),
              (cx-x,cy-y,cz+z),(cx+x,cy-y,cz+z),(cx+x,cy+y,cz+z),(cx-x,cy+y,cz+z)]
    faces=[0,2,1,0,3,2,4,5,6,4,6,7,0,4,7,0,7,3,1,2,6,1,6,5,3,7,6,3,6,2,0,1,5,0,5,4]
    inds += [base+i for i in faces]

def build(name,p):
    v=[];i=[]
    box(v,i,0,.47,0,p['w'],p['h'],p['l'])
    box(v,i,0,.48,p['l']*.5-.12,p['w']*.91,.34,.24)
    box(v,i,0,.48,-p['l']*.5+.12,p['w']*.91,.34,.24)
    box(v,i,0,.76,p['cz'],p['cw'],.30,p['cl']+.28)
    box(v,i,0,1.08,p['cz'],p['cw']*.93,p['ch'],p['cl'])
    box(v,i,0,1.08+p['ch']*.5+.035,p['cz'],p['cw']*.88,.07,p['cl']*.78)
    if name=='small_pickup':
        # Open-bed hint behind the compact cab.
        box(v,i,0,.72,-p['l']*.31,p['w']*.90,.16,p['rear']*.95)
        box(v,i,-p['w']*.43,.91,-p['l']*.31,.10,.38,p['rear']*.95)
        box(v,i,p['w']*.43,.91,-p['l']*.31,.10,.38,p['rear']*.95)
    pos=b''.join(struct.pack('<3f',*q) for q in v);idx=b''.join(struct.pack('<H',q) for q in i);raw=pos+idx
    mins=[min(q[a] for q in v) for a in range(3)];maxs=[max(q[a] for q in v) for a in range(3)]
    uri='data:application/octet-stream;base64,'+base64.b64encode(raw).decode()
    doc={'asset':{'version':'2.0','generator':'Itajai Drive vehicle generator'},'extras':{'itajaiColliderRadius':p['w']*.62},
         'buffers':[{'byteLength':len(raw),'uri':uri}],
         'bufferViews':[{'buffer':0,'byteOffset':0,'byteLength':len(pos),'target':34962},{'buffer':0,'byteOffset':len(pos),'byteLength':len(idx),'target':34963}],
         'accessors':[{'bufferView':0,'componentType':5126,'count':len(v),'type':'VEC3','min':mins,'max':maxs},{'bufferView':1,'componentType':5123,'count':len(i),'type':'SCALAR'}],
         'meshes':[{'name':'car_'+name,'primitives':[{'attributes':{'POSITION':0},'indices':1,'mode':4}]}],
         'nodes':[{'mesh':0}],'scenes':[{'nodes':[0]}],'scene':0}
    return json.dumps(doc,separators=(',',':'))

def main():
    out=pathlib.Path('assets/generated');out.mkdir(parents=True,exist_ok=True)
    for n,p in PRESETS.items():
        path=out/(n+'.gltf');path.write_text(build(n,p),encoding='utf-8');print(path)
if __name__=='__main__':main()
