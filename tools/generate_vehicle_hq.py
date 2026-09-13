#!/usr/bin/env python3
"""Renderer 5.0 vehicle body generator.

Builds smooth original procedural body shells from Brazilian-market proportion
presets. Revision 51 deliberately gives every model a different primary outline
instead of relying on paint/trim to distinguish them: hood/trunk deck, roof edge,
bumper mass, pickup bed and SUV shoulders are baked into the body mesh.
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
    for row,flip in ((0,True),(rows-1,False)):
        z,w,b,t=sections[row]; center=len(v);v.append((0,(b+t)*.5,z));r0=base+row*sides
        for k in range(sides):
            n=(k+1)%sides
            if flip: ind += [center,r0+n,r0+k]
            else: ind += [center,r0+k,r0+n]


def add_family_character(v,ind,p):
    """Large family-level forms which change silhouette at gameplay distance."""
    W,L=p['w'],p['l']; st=p['style']
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
    elif st=='small_hatch':
        box(v,ind,0,.55,L*.46,W*.75,.15,.13);box(v,ind,0,.58,-L*.465,W*.78,.16,.11)
    else:
        box(v,ind,0,.56,L*.46,W*.75,.12,.11);box(v,ind,0,.58,-L*.46,W*.78,.14,.10)
    wb=p['wheelbase'];fz=wb*.5
    for z in (-fz,fz):
        box(v,ind,-W*.465,.49,z,.055,.22,.54)
        box(v,ind, W*.465,.49,z,.055,.22,.54)


def add_model_character(v,ind,name,p):
    """Model-specific silhouette language; no logos/badges are embedded."""
    W,L,H=p['w'],p['l'],p['h']
    if name=='fiat_uno_way_2014':
        # Tall square tail, roof edge and chunky crossover lower corners.
        box(v,ind,0,H*.88,-L*.29,W*.77,.075,L*.24)
        box(v,ind,0,.47,L*.492,W*.88,.22,.095)
        for s in (-1,1): box(v,ind,s*W*.455,.54,0,.075,.20,L*.59)
    elif name=='vw_gol_g6_2016':
        # Low simple hatch: long horizontal bumper and small roof spoiler.
        box(v,ind,0,.49,L*.493,W*.82,.17,.085)
        box(v,ind,0,H*.94,-L*.36,W*.68,.055,.23)
        box(v,ind,0,.62,-L*.485,W*.80,.15,.10)
    elif name=='hyundai_hb20_2024':
        # More tapered nose/rear and a pronounced high rear spoiler.
        box(v,ind,0,.64,L*.475,W*.68,.095,.20)
        box(v,ind,0,H*.91,-L*.355,W*.72,.065,.24)
        for s in (-1,1): box(v,ind,s*W*.39,.58,L*.44,.085,.16,.25)
    elif name=='fiat_strada_2024':
        # Bed is unmistakable even as a silhouette: rails, tailgate and cab break.
        box(v,ind,0,.84,-L*.30,W*.86,.060,L*.31)
        box(v,ind,0,1.18,-L*.105,W*.76,.10,.08)
        box(v,ind,0,.57,L*.48,W*.83,.27,.10)
    elif name=='toyota_corolla_2020':
        # Long sedan hood/deck proportions with a thin trunk lip.
        box(v,ind,0,.66,L*.385,W*.76,.10,L*.18)
        box(v,ind,0,.73,-L*.405,W*.80,.11,L*.19)
        box(v,ind,0,.83,-L*.485,W*.58,.045,.15)
    elif name=='jeep_renegade_2021':
        # Upright square SUV body with strong four-corner shoulder masses.
        box(v,ind,0,.61,L*.485,W*.91,.36,.095)
        box(v,ind,0,.62,-L*.485,W*.92,.37,.095)
        for s in (-1,1):
            box(v,ind,s*W*.46,.58,L*.31,.080,.32,.52)
            box(v,ind,s*W*.46,.58,-L*.31,.080,.32,.52)
    elif name=='chevrolet_onix_2024':
        # Longer modern hatch with lower nose and fast rear roof edge.
        box(v,ind,0,.57,L*.482,W*.72,.11,.13)
        box(v,ind,0,H*.91,-L*.37,W*.67,.050,.25)
        box(v,ind,0,.63,-L*.48,W*.78,.13,.10)
    elif name=='chevrolet_celta_2015':
        # Very short, simple old-school hatch with visibly chunky bumpers.
        box(v,ind,0,.46,L*.492,W*.86,.22,.095)
        box(v,ind,0,.48,-L*.492,W*.87,.23,.095)
        box(v,ind,0,H*.90,-L*.30,W*.58,.045,.16)


def build(name,p):
    v=[];ind=[];W,L=p['w'],p['l']
    lower=[(zf*L,wf*W*.5,.26,top) for zf,wf,top in p['lower']]
    roof=[(zf*L,wf*W*.5,base,top) for zf,wf,base,top in p['roof']]
    lower=resample(lower,19);roof=resample(roof,13)
    rounded_shell(v,ind,lower,16,.66)
    rounded_shell(v,ind,roof,14,.73)
    add_family_character(v,ind,p)
    add_model_character(v,ind,name,p)
    pos=b''.join(struct.pack('<3f',*q) for q in v)
    idx=b''.join(struct.pack('<H',q) for q in ind)
    raw=pos+idx
    mins=[min(q[a] for q in v) for a in range(3)];maxs=[max(q[a] for q in v) for a in range(3)]
    uri='data:application/octet-stream;base64,'+base64.b64encode(raw).decode()
    doc={
      'asset':{'version':'2.0','generator':'Itajai Drive Renderer 5.0 HQ vehicle generator'},
      'extras':{'itajaiColliderRadius':W*.62,'realWorldReference':name,'wheelbase':p['wheelbase'],'silhouetteRevision':51,'smoothNormals':True,'smoothBody':True},
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
        path=out/(name+'.gltf');path.write_text(build(name,p),encoding='utf-8');print('HQ51',path)

if __name__=='__main__':
    main()
