"""Original Novo Uno shell: shaped side panels, open wheel arches and inset glazing.
Coordinates are metres, +Z forward. No simulation dimensions are changed.
"""
import math

# z, half width, shoulder height. Dense stations preserve the short bonnet and
# nearly vertical hatch; wheel openings are actual boundaries, never black decals.
STATIONS=[(-1.905,.67,.76),(-1.86,.75,.85),(-1.72,.795,.94),(-1.52,.82,.965),(-1.19,.82,.97),(-.82,.813,.965),(-.40,.811,.96),(0,.813,.96),(.40,.816,.955),(.78,.815,.94),(1.19,.809,.915),(1.48,.795,.875),(1.72,.755,.825),(1.86,.70,.76),(1.905,.65,.72)]

def lerp_station(z):
    for a,b in zip(STATIONS,STATIONS[1:]):
        if a[0]<=z<=b[0]:
            t=(z-a[0])/(b[0]-a[0]);return a[1]+t*(b[1]-a[1]),a[2]+t*(b[2]-a[2])
    return STATIONS[-1][1:]

def quad(v,i,a,b,c,d):
    n=len(v);v.extend((a,b,c,d));i.extend((n,n+1,n+2,n,n+2,n+3))

def body(v,i):
    zs={q[0] for q in STATIONS}
    for wheel in (-1.19,1.19):
        for k in range(25):zs.add(wheel+.365*math.cos(math.pi*k/24))
    zs=sorted(zs)
    for s in (-1,1):
        rows=[]
        for z in zs:
            w,top=lerp_station(z);bottom=.29
            for wheel in (-1.19,1.19):
                d=abs(z-wheel)
                if d<=.365:bottom=max(bottom,.34+math.sqrt(max(0,.365**2-d*d)))
            rows.append([(s*(w-.045),bottom,z),(s*(w+.002),bottom+(top-bottom)*.26,z),(s*w,top-.055,z),(s*(w-.035),top,z)])
        for a,b in zip(rows,rows[1:]):
            for k in range(3):
                pts=(a[k],b[k],b[k+1],a[k+1]);quad(v,i,*(pts if s<0 else pts[::-1]))
    # Bonnet and boot are curved cross-car strips, with proper outward winding.
    for a,b in zip(zs,zs[1:]):
        if a< -1.70 or a>=.78:
            wa,ha=lerp_station(a);wb,hb=lerp_station(b)
            for k in range(12):
                x0=-1+k/6;x1=x0+1/6
                quad(v,i,(x0*(wa-.035),ha+.035*(1-x0*x0),a),(x0*(wb-.035),hb+.035*(1-x0*x0),b),(x1*(wb-.035),hb+.035*(1-x1*x1),b),(x1*(wa-.035),ha+.035*(1-x1*x1),a))
    # Tall cabin: broad crowned roof, raked A pillar, upright C pillar.
    roof=[(-1.70,.69,1.31),(-1.58,.708,1.46),(-1.36,.72,1.525),(-.90,.724,1.55),(-.40,.724,1.55),(.10,.712,1.535),(.35,.697,1.49)]
    for a,b in zip(roof,roof[1:]):
        for k in range(16):
            x0=-1+k/8;x1=x0+.125
            point=lambda row,x:(row[1]*x,row[2]-.045*x*x,row[0])
            quad(v,i,point(a,x0),point(b,x0),point(b,x1),point(a,x1))
    # Painted posts and belt frame around (not behind) separate panes.
    for s in (-1,1):
        def p(z,y):return (s*(.805-(y-.96)*.20),y,z)
        for z0,z1,y0,y1 in [(-1.70,-1.50,1.31,1.45),(-.22,-.13,1.505,1.505),(.30,.82,1.455,.955)]:
            pts=(p(z0,.955),p(z1,.955),p(z1,y1),p(z0,y0));quad(v,i,*(pts if s<0 else pts[::-1]))
        quad(v,i,p(-1.70,.91),p(.82,.91),p(.82,.965),p(-1.70,.965))
    for z,w,h in (STATIONS[0],STATIONS[-1]):
        pts=((-w,.29,z),(w,.29,z),(w,h,z),(-w,h,z));quad(v,i,*(pts if z>0 else pts[::-1]))
    quad(v,i,(-.76,.76,-1.85),(-.69,1.31,-1.70),(.69,1.31,-1.70),(.76,.76,-1.85))

def glass():
    v=[];i=[]
    for s in (-1,1):
        def p(z,y):return (s*(.812-(y-.96)*.20),y,z)
        for corners in [((-1.49,.988),(-.265,.988),(-.265,1.477),(-1.48,1.417)),((-.095,.988),(.733,.978),(.302,1.422),(-.095,1.475))]:
            pts=tuple(p(z,y) for z,y in corners);quad(v,i,*(pts if s<0 else pts[::-1]))
    quad(v,i,(-.738,.978,.792),(.738,.978,.792),(.663,1.435,.358),(-.663,1.435,.358))
    quad(v,i,(-.666,1.025,-1.785),(-.64,1.345,-1.703),(.64,1.345,-1.703),(.666,1.025,-1.785))
    return v,i

def trim():
    from generate_vehicle_parts import box
    v=[];i=[]
    for s in (-1,1):
        box(v,i,s*.815,.415,0,.055,.19,1.64)
        for wheel in (-1.19,1.19):
            for k in range(24):
                a=math.pi*k/24;b=math.pi*(k+1)/24
                def p(r,t):return (s*.824,.34+r*math.sin(t),wheel+r*math.cos(t))
                pts=(p(.366,a),p(.412,a),p(.412,b),p(.366,b));quad(v,i,*(pts if s>0 else pts[::-1]))
        box(v,i,s*.55,1.585,-.55,.045,.045,1.84)
        for z in (-1.30,.20):box(v,i,s*.55,1.54,z,.065,.07,.13)
    box(v,i,0,.46,1.895,1.40,.27,.08);box(v,i,0,.445,-1.904,1.46,.25,.07)
    return v,i
