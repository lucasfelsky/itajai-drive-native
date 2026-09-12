#!/usr/bin/env python3
"""Generate vehicle sub-assets for the 2.10 named-part contract.

The existing body generator remains authoritative. This companion writes glass and
trim meshes beside each body so the runtime can resolve independent materials and
future hand-authored glTF replacements without changing simulation code.
"""
import base64, json, pathlib, struct
import generate_vehicle_gltf as bodygen


def quad(v,i,a,b,c,d):
    n=len(v); v += [a,b,c,d]; i += [n,n+1,n+2,n,n+2,n+3]

def box(v,i,cx,cy,cz,sx,sy,sz):
    x=sx*.5;y=sy*.5;z=sz*.5
    p=[(cx-x,cy-y,cz-z),(cx+x,cy-y,cz-z),(cx+x,cy+y,cz-z),(cx-x,cy+y,cz-z),
       (cx-x,cy-y,cz+z),(cx+x,cy-y,cz+z),(cx+x,cy+y,cz+z),(cx-x,cy+y,cz+z)]
    quad(v,i,p[0],p[1],p[2],p[3]);quad(v,i,p[4],p[7],p[6],p[5]);quad(v,i,p[0],p[4],p[5],p[1]);quad(v,i,p[3],p[2],p[6],p[7]);quad(v,i,p[0],p[3],p[7],p[4]);quad(v,i,p[1],p[5],p[6],p[2])

def loft(v,i,sections):
    for a,b in zip(sections,sections[1:]):
        za,wa,ba,ta=a;zb,wb,bb,tb=b
        quad(v,i,(-wa,ba,za),(-wb,bb,zb),(-wb,tb,zb),(-wa,ta,za))
        quad(v,i,(wa,ba,za),(wa,ta,za),(wb,tb,zb),(wb,bb,zb))
        quad(v,i,(-wa,ta,za),(-wb,tb,zb),(wb,tb,zb),(wa,ta,za))
    za,wa,ba,ta=sections[0];quad(v,i,(-wa,ba,za),(-wa,ta,za),(wa,ta,za),(wa,ba,za))
    za,wa,ba,ta=sections[-1];quad(v,i,(-wa,ba,za),(wa,ba,za),(wa,ta,za),(-wa,ta,za))

def write(path,name,v,i,collider=0.0):
    pos=b''.join(struct.pack('<3f',*q) for q in v); idx=b''.join(struct.pack('<H',q) for q in i); raw=pos+idx
    mins=[min(q[a] for q in v) for a in range(3)]; maxs=[max(q[a] for q in v) for a in range(3)]
    uri='data:application/octet-stream;base64,'+base64.b64encode(raw).decode()
    doc={'asset':{'version':'2.0','generator':'Itajai Drive named vehicle parts 2.10'},'extras':{'itajaiColliderRadius':collider},
         'buffers':[{'byteLength':len(raw),'uri':uri}],
         'bufferViews':[{'buffer':0,'byteOffset':0,'byteLength':len(pos),'target':34962},{'buffer':0,'byteOffset':len(pos),'byteLength':len(idx),'target':34963}],
         'accessors':[{'bufferView':0,'componentType':5126,'count':len(v),'type':'VEC3','min':mins,'max':maxs},{'bufferView':1,'componentType':5123,'count':len(i),'type':'SCALAR'}],
         'meshes':[{'name':name,'primitives':[{'attributes':{'POSITION':0},'indices':1,'mode':4}]}],'nodes':[{'mesh':0}],'scenes':[{'nodes':[0]}],'scene':0}
    path.write_text(json.dumps(doc,separators=(',',':')),encoding='utf-8')

def glass_for(p):
    v=[];i=[];L=p['l'];W=p['w']
    # 2.2+ body presets expose six explicit roof stations: z,width,bottom,top.
    # Inset them slightly so glass is a separate shell rather than z-fighting body metal.
    sections=[]
    for zf,wf,base,top in p['roof']:
        sections.append((zf*L,wf*W*.465,base+.035,max(base+.08,top-.055)))
    loft(v,i,sections);return v,i

def trim_for(p):
    v=[];i=[];W=p['w'];L=p['l'];H=p['h'];style=p['style'];fy=.64+(H-1.43)*.06
    box(v,i,0,.46,L*.497,W*.86,.16,.08);box(v,i,0,.46,-L*.497,W*.88,.16,.08)
    for s in (-1,1):
        box(v,i,s*W*.31,fy,L*.505,W*.18,.16,.05);box(v,i,s*W*.31,.65,-L*.505,W*.17,.17,.05)
    box(v,i,0,.43,L*.507,W*.40,.18,.045)
    if style=='box_suv':
        for k in range(-3,4): box(v,i,k*.105,.61,L*.510,.045,.24,.035)
    if style=='pickup': box(v,i,0,.72,-L*.30,W*.79,.06,L*.31)
    if style=='sedan': box(v,i,0,.57,L*.509,W*.52,.075,.035)
    return v,i

def main():
    out=pathlib.Path('assets/generated');out.mkdir(parents=True,exist_ok=True)
    for name,p in bodygen.PRESETS.items():
        v,i=glass_for(p);write(out/(name+'_glass.gltf'),'glass_'+name,v,i,0.0)
        v,i=trim_for(p);write(out/(name+'_trim.gltf'),'trim_'+name,v,i,0.0)
        print(out/(name+'_glass.gltf'));print(out/(name+'_trim.gltf'))

if __name__=='__main__':main()
