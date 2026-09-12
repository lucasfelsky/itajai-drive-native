#!/usr/bin/env python3
"""Generate recognizable low/mid-poly Brazilian vehicle bodies for Itajai Drive.

The sources are original procedural meshes built from dimensional/silhouette presets;
no manufacturer mesh or logo asset is copied.  Names are useful inside this private
prototype so each handling/visual profile is easy to identify.

Wheels, glass, lights and trim are layered by the native renderer.  The generated
body mesh provides the characteristic proportions: hood/trunk profile, cabin rake,
boxiness, pickup bed and overall dimensions.
"""
import base64, json, pathlib, struct

# z points forward in the native engine. Dimensions are intentionally approximate:
# the goal is recognizable proportion/style rather than CAD reconstruction.
PRESETS = {
    'fiat_uno_way_2014': dict(
        w=1.64,l=3.81,h=1.55,wheelbase=2.38,style='box_hatch',
        lower=[(-.50,.92,.76),(-.38,.99,.82),(.10,1.00,.84),(.39,.96,.76),(.50,.86,.72)],
        cabin=(-.34,.31,.94,1.50,.94,.87)),
    'vw_gol_g6_2016': dict(
        w=1.66,l=3.90,h=1.46,wheelbase=2.47,style='hatch',
        lower=[(-.50,.90,.68),(-.40,.99,.73),(.10,1.00,.76),(.38,.98,.70),(.50,.88,.66)],
        cabin=(-.38,.27,.90,1.42,.93,.84)),
    'hyundai_hb20_2024': dict(
        w=1.72,l=4.02,h=1.47,wheelbase=2.53,style='modern_hatch',
        lower=[(-.50,.86,.67),(-.40,.98,.72),(.12,1.00,.77),(.40,.96,.69),(.50,.82,.63)],
        cabin=(-.36,.28,.88,1.43,.92,.82)),
    'fiat_strada_2024': dict(
        w=1.73,l=4.48,h=1.61,wheelbase=2.74,style='pickup',
        lower=[(-.50,.93,.69),(-.42,.99,.73),(.05,1.00,.76),(.39,.96,.72),(.50,.86,.67)],
        cabin=(-.03,.33,.92,1.54,.92,.82)),
    'toyota_corolla_2020': dict(
        w=1.78,l=4.63,h=1.45,wheelbase=2.70,style='sedan',
        lower=[(-.50,.84,.65),(-.39,.98,.70),(.05,1.00,.75),(.40,.96,.67),(.50,.79,.61)],
        cabin=(-.30,.25,.86,1.40,.91,.80)),
    'jeep_renegade_2021': dict(
        w=1.81,l=4.23,h=1.67,wheelbase=2.57,style='box_suv',
        lower=[(-.50,.91,.78),(-.42,.99,.84),(.08,1.00,.88),(.42,.97,.80),(.50,.89,.76)],
        cabin=(-.39,.31,.95,1.61,.96,.90)),
    'chevrolet_onix_2024': dict(
        w=1.73,l=4.16,h=1.48,wheelbase=2.55,style='modern_hatch',
        lower=[(-.50,.84,.66),(-.40,.98,.72),(.10,1.00,.77),(.40,.96,.68),(.50,.82,.62)],
        cabin=(-.36,.29,.88,1.44,.92,.82)),
    'chevrolet_celta_2015': dict(
        w=1.63,l=3.79,h=1.43,wheelbase=2.44,style='small_hatch',
        lower=[(-.50,.88,.66),(-.40,.98,.71),(.08,1.00,.74),(.39,.96,.68),(.50,.85,.63)],
        cabin=(-.39,.27,.90,1.39,.92,.83)),
}

def quad(v, i, a, b, c, d):
    base = len(v)
    v += [a,b,c,d]
    i += [base,base+1,base+2, base,base+2,base+3]

def box(v, i, cx, cy, cz, sx, sy, sz):
    x=sx*.5; y=sy*.5; z=sz*.5
    p=[(cx-x,cy-y,cz-z),(cx+x,cy-y,cz-z),(cx+x,cy+y,cz-z),(cx-x,cy+y,cz-z),
       (cx-x,cy-y,cz+z),(cx+x,cy-y,cz+z),(cx+x,cy+y,cz+z),(cx-x,cy+y,cz+z)]
    quad(v,i,p[0],p[1],p[2],p[3]); quad(v,i,p[4],p[7],p[6],p[5])
    quad(v,i,p[0],p[4],p[5],p[1]); quad(v,i,p[3],p[2],p[6],p[7])
    quad(v,i,p[0],p[3],p[7],p[4]); quad(v,i,p[1],p[5],p[6],p[2])

def loft(v, i, stations, y_bottom):
    """Closed faceted shell. station = (z, half_width, top_y)."""
    for a,b in zip(stations, stations[1:]):
        za,wa,ta=a; zb,wb,tb=b
        # left/right body sides
        quad(v,i,(-wa,y_bottom,za),(-wb,y_bottom,zb),(-wb,tb,zb),(-wa,ta,za))
        quad(v,i,( wa,y_bottom,za),( wa,ta,za),( wb,tb,zb),( wb,y_bottom,zb))
        # upper body surface and underside
        quad(v,i,(-wa,ta,za),(-wb,tb,zb),(wb,tb,zb),(wa,ta,za))
        quad(v,i,(-wa,y_bottom,za),(wa,y_bottom,za),(wb,y_bottom,zb),(-wb,y_bottom,zb))
    z,w,t=stations[0]; quad(v,i,(-w,y_bottom,z),(-w,t,z),(w,t,z),(w,y_bottom,z))
    z,w,t=stations[-1]; quad(v,i,(-w,y_bottom,z),(w,y_bottom,z),(w,t,z),(-w,t,z))

def cabin(v, i, p):
    rear,front,width_frac,height,width_roof,base_frac=p['cabin']
    L=p['l']; W=p['w']; z0=rear*L; z3=front*L
    span=z3-z0
    # Four sections create rear glass, roof plateau and windshield rake.
    sections=[
        (z0, W*width_frac*.48, .72, .93),
        (z0+span*.22, W*width_roof*.48, .77, height),
        (z0+span*.72, W*width_roof*.47, .77, height-.01),
        (z3, W*base_frac*.48, .71, .94),
    ]
    for a,b in zip(sections,sections[1:]):
        za,wa,ba,ta=a; zb,wb,bb,tb=b
        quad(v,i,(-wa,ba,za),(-wb,bb,zb),(-wb,tb,zb),(-wa,ta,za))
        quad(v,i,(wa,ba,za),(wa,ta,za),(wb,tb,zb),(wb,bb,zb))
        quad(v,i,(-wa,ta,za),(-wb,tb,zb),(wb,tb,zb),(wa,ta,za))
    za,wa,ba,ta=sections[0]; quad(v,i,(-wa,ba,za),(-wa,ta,za),(wa,ta,za),(wa,ba,za))
    za,wa,ba,ta=sections[-1]; quad(v,i,(-wa,ba,za),(wa,ba,za),(wa,ta,za),(-wa,ta,za))

def build(name,p):
    v=[]; i=[]; W=p['w']; L=p['l']; style=p['style']
    stations=[(zf*L, wf*W*.5, top) for zf,wf,top in p['lower']]
    loft(v,i,stations,.27)
    cabin(v,i,p)

    # Character pieces are body-coloured; trim/glass/lights come from native layer.
    if style in ('box_hatch','box_suv'):
        box(v,i,0,.55,L*.475,W*.78,.34,.15)   # upright front fascia
        box(v,i,0,.58,-L*.475,W*.80,.36,.15)  # upright tailgate
    if style=='sedan':
        box(v,i,0,.70,-L*.365,W*.82,.18,L*.22) # defined trunk deck
    if style=='pickup':
        # Bed floor and body-colour rails make the silhouette unmistakably pickup.
        bedz=-L*.305; bedlen=L*.34
        box(v,i,0,.52,bedz,W*.88,.12,bedlen)
        box(v,i,-W*.455,.78,bedz,.11,.48,bedlen)
        box(v,i, W*.455,.78,bedz,.11,.48,bedlen)
        box(v,i,0,.78,-L*.468,W*.91,.48,.11)
    if style=='modern_hatch':
        box(v,i,0,.60,-L*.455,W*.84,.22,.16)
    if style=='small_hatch':
        box(v,i,0,.58,L*.455,W*.78,.24,.14)

    pos=b''.join(struct.pack('<3f',*q) for q in v)
    # Independent quad vertices keep faceted normals crisp in compile_assets.py.
    idx=b''.join(struct.pack('<H',q) for q in i)
    raw=pos+idx
    mins=[min(q[a] for q in v) for a in range(3)]
    maxs=[max(q[a] for q in v) for a in range(3)]
    uri='data:application/octet-stream;base64,'+base64.b64encode(raw).decode()
    doc={
        'asset':{'version':'2.0','generator':'Itajai Drive 1.6 vehicle generator'},
        'extras':{'itajaiColliderRadius':W*.62,'realWorldReference':name,'wheelbase':p['wheelbase']},
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
    # Remove obsolete generated glTFs so a local developer cannot mistake them for
    # current assets. CI passes an explicit file list either way.
    for old in out.glob('*.gltf'):
        old.unlink()
    for n,p in PRESETS.items():
        path=out/(n+'.gltf'); path.write_text(build(n,p),encoding='utf-8'); print(path)

if __name__=='__main__': main()
