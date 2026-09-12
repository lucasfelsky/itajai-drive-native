#!/usr/bin/env python3
"""Generate the Itajai Drive Brazilian vehicle body meshes.

2.2 increases silhouette fidelity substantially: each model now has an explicit
nine-station lower body and six-station roof/cabin profile instead of sharing a
near-identical five-station hatch shell. The geometry is still original procedural
work made for this prototype; manufacturer badges/logos are not embedded.

z points forward in the native renderer. Wheels, glass, lamps, plates and trim are
layered by C at runtime; this file owns the recognisable body/roof silhouette.
"""
import base64, json, pathlib, struct

PRESETS = {
    'fiat_uno_way_2014': dict(
        w=1.64,l=3.81,h=1.55,wheelbase=2.38,style='box_hatch',
        lower=[(-.50,.83,.70),(-.47,.94,.78),(-.39,1.00,.84),(-.22,1.00,.87),(.05,1.00,.88),(.25,.99,.87),(.40,.98,.84),(.48,.92,.78),(.50,.82,.72)],
        roof=[(-.405,.88,.75,1.25),(-.34,.94,.78,1.49),(-.18,.96,.80,1.55),(.13,.96,.80,1.55),(.30,.94,.78,1.52),(.365,.88,.75,1.28)]),
    'vw_gol_g6_2016': dict(
        w=1.66,l=3.90,h=1.46,wheelbase=2.47,style='hatch',
        lower=[(-.50,.82,.64),(-.47,.92,.69),(-.39,.99,.75),(-.22,1.00,.79),(.05,1.00,.80),(.26,.99,.78),(.40,.96,.73),(.47,.91,.68),(.50,.80,.64)],
        roof=[(-.39,.82,.71,1.05),(-.31,.91,.75,1.37),(-.15,.93,.77,1.45),(.09,.92,.77,1.45),(.24,.88,.74,1.40),(.31,.78,.70,1.08)]),
    'hyundai_hb20_2024': dict(
        w=1.72,l=4.02,h=1.47,wheelbase=2.53,style='modern_hatch',
        lower=[(-.50,.72,.59),(-.47,.85,.64),(-.40,.97,.72),(-.24,1.00,.79),(.00,1.00,.82),(.23,.98,.79),(.39,.91,.70),(.47,.78,.62),(.50,.66,.57)],
        roof=[(-.37,.74,.70,1.01),(-.28,.88,.75,1.34),(-.10,.92,.78,1.46),(.08,.90,.78,1.47),(.23,.83,.74,1.38),(.30,.70,.68,1.06)]),
    'fiat_strada_2024': dict(
        w=1.73,l=4.48,h=1.61,wheelbase=2.74,style='pickup',
        lower=[(-.50,.88,.69),(-.47,.96,.73),(-.39,1.00,.78),(-.18,1.00,.80),(.07,1.00,.80),(.29,.99,.79),(.41,.96,.74),(.48,.88,.67),(.50,.78,.62)],
        roof=[(-.08,.84,.76,1.08),(.00,.91,.79,1.48),(.10,.93,.80,1.59),(.23,.92,.80,1.60),(.32,.88,.78,1.52),(.38,.77,.72,1.14)]),
    'toyota_corolla_2020': dict(
        w=1.78,l=4.63,h=1.45,wheelbase=2.70,style='sedan',
        lower=[(-.50,.75,.59),(-.47,.88,.63),(-.39,.98,.69),(-.20,1.00,.76),(.02,1.00,.79),(.24,.99,.76),(.40,.94,.68),(.48,.80,.59),(.50,.69,.56)],
        roof=[(-.31,.76,.70,.96),(-.22,.88,.74,1.27),(-.08,.91,.77,1.42),(.10,.90,.77,1.44),(.25,.84,.73,1.30),(.33,.70,.67,1.00)]),
    'jeep_renegade_2021': dict(
        w=1.81,l=4.23,h=1.67,wheelbase=2.57,style='box_suv',
        lower=[(-.50,.85,.73),(-.48,.94,.80),(-.42,1.00,.88),(-.24,1.00,.92),(.03,1.00,.93),(.28,.99,.91),(.42,.98,.87),(.48,.92,.81),(.50,.82,.75)],
        roof=[(-.40,.88,.82,1.29),(-.35,.94,.86,1.61),(-.20,.96,.88,1.67),(.13,.96,.88,1.67),(.31,.94,.86,1.64),(.37,.87,.81,1.32)]),
    'chevrolet_onix_2024': dict(
        w=1.73,l=4.16,h=1.48,wheelbase=2.55,style='modern_hatch',
        lower=[(-.50,.73,.59),(-.47,.86,.64),(-.40,.98,.72),(-.23,1.00,.79),(.02,1.00,.82),(.25,.98,.78),(.40,.90,.69),(.47,.78,.62),(.50,.67,.58)],
        roof=[(-.37,.75,.70,1.02),(-.28,.88,.75,1.34),(-.11,.92,.78,1.46),(.08,.91,.78,1.48),(.24,.84,.74,1.39),(.31,.71,.68,1.06)]),
    'chevrolet_celta_2015': dict(
        w=1.63,l=3.79,h=1.43,wheelbase=2.44,style='small_hatch',
        lower=[(-.50,.80,.62),(-.47,.91,.67),(-.39,.99,.73),(-.22,1.00,.77),(.04,1.00,.78),(.25,.98,.75),(.39,.94,.69),(.47,.87,.64),(.50,.77,.60)],
        roof=[(-.40,.80,.69,1.01),(-.32,.89,.73,1.31),(-.16,.92,.76,1.41),(.08,.91,.76,1.42),(.25,.86,.72,1.35),(.32,.75,.67,1.03)]),
}


def quad(v, i, a, b, c, d):
    base=len(v); v += [a,b,c,d]; i += [base,base+1,base+2, base,base+2,base+3]


def box(v, i, cx, cy, cz, sx, sy, sz):
    x=sx*.5; y=sy*.5; z=sz*.5
    p=[(cx-x,cy-y,cz-z),(cx+x,cy-y,cz-z),(cx+x,cy+y,cz-z),(cx-x,cy+y,cz-z),
       (cx-x,cy-y,cz+z),(cx+x,cy-y,cz+z),(cx+x,cy+y,cz+z),(cx-x,cy+y,cz+z)]
    quad(v,i,p[0],p[1],p[2],p[3]); quad(v,i,p[4],p[7],p[6],p[5])
    quad(v,i,p[0],p[4],p[5],p[1]); quad(v,i,p[3],p[2],p[6],p[7])
    quad(v,i,p[0],p[3],p[7],p[4]); quad(v,i,p[1],p[5],p[6],p[2])


def shell(v, i, sections, close_bottom=True):
    """Faceted closed shell: section=(z, half_width, bottom_y, top_y)."""
    for a,b in zip(sections,sections[1:]):
        za,wa,ba,ta=a; zb,wb,bb,tb=b
        quad(v,i,(-wa,ba,za),(-wb,bb,zb),(-wb,tb,zb),(-wa,ta,za))
        quad(v,i,( wa,ba,za),( wa,ta,za),( wb,tb,zb),( wb,bb,zb))
        quad(v,i,(-wa,ta,za),(-wb,tb,zb),( wb,tb,zb),( wa,ta,za))
        if close_bottom: quad(v,i,(-wa,ba,za),(wa,ba,za),(wb,bb,zb),(-wb,bb,zb))
    z,w,b,t=sections[0]; quad(v,i,(-w,b,z),(-w,t,z),(w,t,z),(w,b,z))
    z,w,b,t=sections[-1]; quad(v,i,(-w,b,z),(w,b,z),(w,t,z),(-w,t,z))


def build(name,p):
    v=[]; i=[]; W=p['w']; L=p['l']; style=p['style']
    lower=[(zf*L,wf*W*.5,.27,top) for zf,wf,top in p['lower']]
    roof=[(zf*L,wf*W*.5,base,top) for zf,wf,base,top in p['roof']]
    shell(v,i,lower,True); shell(v,i,roof,False)

    # Model-family character volumes deliberately alter outline, not just surface detail.
    if style=='box_hatch':
        box(v,i,0,.51,L*.474,W*.80,.31,.18); box(v,i,0,.57,-L*.476,W*.84,.38,.16)
        box(v,i,0,1.48,-L*.20,W*.74,.08,L*.26)  # long upright roof character
    elif style=='box_suv':
        box(v,i,0,.59,L*.477,W*.86,.43,.18); box(v,i,0,.62,-L*.478,W*.88,.46,.18)
        box(v,i,-W*.43,.48,0,.10,.25,L*.66); box(v,i,W*.43,.48,0,.10,.25,L*.66)
    elif style=='sedan':
        box(v,i,0,.70,-L*.385,W*.84,.17,L*.19); box(v,i,0,.63,L*.405,W*.80,.16,L*.14)
    elif style=='pickup':
        bedz=-L*.315; bedlen=L*.35
        box(v,i,0,.51,bedz,W*.90,.11,bedlen)
        box(v,i,-W*.455,.77,bedz,.11,.51,bedlen); box(v,i,W*.455,.77,bedz,.11,.51,bedlen)
        box(v,i,0,.77,-L*.475,W*.92,.50,.12)
    elif style=='modern_hatch':
        box(v,i,0,.59,-L*.462,W*.80,.16,.15); box(v,i,0,.55,L*.455,W*.73,.12,.18)
    elif style=='small_hatch':
        box(v,i,0,.56,L*.455,W*.76,.20,.15); box(v,i,0,.61,-L*.455,W*.79,.18,.14)
    else:  # conventional hatch
        box(v,i,0,.58,L*.458,W*.77,.18,.15); box(v,i,0,.60,-L*.462,W*.80,.20,.14)

    pos=b''.join(struct.pack('<3f',*q) for q in v)
    idx=b''.join(struct.pack('<H',q) for q in i)
    raw=pos+idx
    mins=[min(q[a] for q in v) for a in range(3)]; maxs=[max(q[a] for q in v) for a in range(3)]
    uri='data:application/octet-stream;base64,'+base64.b64encode(raw).decode()
    doc={
        'asset':{'version':'2.0','generator':'Itajai Drive 2.2 vehicle silhouette generator'},
        'extras':{'itajaiColliderRadius':W*.62,'realWorldReference':name,'wheelbase':p['wheelbase'],'silhouetteRevision':22},
        'buffers':[{'byteLength':len(raw),'uri':uri}],
        'bufferViews':[{'buffer':0,'byteOffset':0,'byteLength':len(pos),'target':34962},
                       {'buffer':0,'byteOffset':len(pos),'byteLength':len(idx),'target':34963}],
        'accessors':[{'bufferView':0,'componentType':5126,'count':len(v),'type':'VEC3','min':mins,'max':maxs},
                     {'bufferView':1,'componentType':5123,'count':len(i),'type':'SCALAR'}],
        'meshes':[{'name':'car_'+name,'primitives':[{'attributes':{'POSITION':0},'indices':1,'mode':4}]}],
        'nodes':[{'mesh':0}],'scenes':[{'nodes':[0]}],'scene':0
    }
    return json.dumps(doc,separators=(',',':'))


def main():
    out=pathlib.Path('assets/generated'); out.mkdir(parents=True,exist_ok=True)
    for old in out.glob('*.gltf'): old.unlink()
    for n,p in PRESETS.items():
        path=out/(n+'.gltf'); path.write_text(build(n,p),encoding='utf-8'); print(path)


if __name__=='__main__': main()
