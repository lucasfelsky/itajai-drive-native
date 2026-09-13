"""Original low-cost coastal vegetation; preserves existing asset/collider contracts."""
import math,pathlib
from generate_vehicle_parts import write

def ellipsoid(v,ind,c,r,phase=0):
    base=len(v);rings=6;sides=10
    for j in range(rings+1):
        t=math.pi*j/rings
        for k in range(sides):
            a=k*math.tau/sides;rough=1+.075*math.sin(k*7+j*3+phase)
            v.append((c[0]+r[0]*math.sin(t)*math.cos(a)*rough,c[1]+r[1]*math.cos(t),c[2]+r[2]*math.sin(t)*math.sin(a)*rough))
    for j in range(rings):
        for k in range(sides):
            a=base+j*sides+k;b=base+j*sides+(k+1)%sides;c1=b+sides;d=a+sides
            if j:ind.extend((a,b,d))
            if j<rings-1:ind.extend((b,c1,d))

def trunk(v,i,height,radius):
    base=len(v)
    for y,r in ((0,radius),(height,radius*.48)):
        for k in range(9):
            a=k*math.tau/9;v.append((r*math.cos(a)+y*.018,y,r*math.sin(a)))
    for k in range(9):
        a=base+k;b=base+(k+1)%9;i.extend((a,b,b+9,a,b+9,a+9))

def main():
    v=[];i=[];trunk(v,i,3.35,.18)
    for k in range(5):
        a=k*math.tau/5;ellipsoid(v,i,(math.cos(a)*.64,2.8+(k%2)*.36,math.sin(a)*.64),(.94,1.05,.92),k)
    ellipsoid(v,i,(.05,3.65,0),(.90,1.0,.83),7)
    write(pathlib.Path('assets/source/tree.gltf'),'urban_tree',v,i,.55,True)
    v=[];i=[];trunk(v,i,5.5,.18)
    for k in range(11):
        a=k*math.tau/11
        for j in range(6):
            t0=j/6;t1=(j+1)/6
            def p(t,side):
                dist=2.65*t;w=.40*math.sin(math.pi*t)
                return (math.cos(a)*dist-math.sin(a)*w*side,5.5+.6*math.sin(math.pi*t)-1.5*t*t,math.sin(a)*dist+math.cos(a)*w*side)
            n=len(v);v.extend((p(t0,-1),p(t1,-1),p(t1,1),p(t0,1)));i.extend((n,n+1,n+2,n,n+2,n+3,n+2,n+1,n,n+3,n+2,n))
    write(pathlib.Path('assets/source/palm.gltf'),'coastal_palm',v,i,.48,True)
if __name__=='__main__':main()
