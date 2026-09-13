"""Check hero geometry and all runtime asset contracts before publishing."""
import math,pathlib,sys
from compile_assets import compile_one
from generate_vehicle_gltf import PRESETS
from release_check import check_version

def main():
    check_version()
    names=set()
    for p in list(pathlib.Path('assets/source').glob('*.gltf'))+list(pathlib.Path('assets/generated').glob('*.gltf')):
        name,v,i,collider,lo,hi=compile_one(p)
        assert name not in names, ('duplicate asset',name)
        names.add(name)
        assert len(name.encode())<=31, ('runtime name truncation',name)
        assert all(math.isfinite(x) for q in v for x in q), ('nonfinite vertex',p)
        assert i and min(i)>=0 and max(i)<len(v), ('invalid index',p)
        assert collider>=0 and all(b>a for a,b in zip(lo,hi)), ('invalid bounds',p)
    expected={prefix+name for prefix in ('car_','glass_','trim_') for name in PRESETS}
    assert expected<=names, ('missing vehicle parts',expected-names)
    assert len(names)==32, ('native MAX_ASSETS08 contract',len(names))
    name,v,i,_,lo,hi=compile_one(pathlib.Path('assets/generated/fiat_uno_way_2014.gltf'))
    # The upper wheel arches must be cut into the side surface; they cannot be
    # a black trim decal pasted onto the old closed body.
    for wheel in (-1.19,1.19):
        side=[p for p in v if abs(p[0])>.76 and abs(p[2]-wheel)<.04]
        assert side and min(p[1] for p in side)>.67, ('wheel opening filled',wheel)
    _,glass,_,_,_,_=compile_one(pathlib.Path('assets/generated/fiat_uno_way_2014_glass.gltf'))
    assert max(p[2] for p in glass)<.85, 'windscreen must end at the short bonnet'
    assert max(p[1] for p in glass)<1.5, 'glass must not cover painted roof'
    assert len(i)//3<2200, 'hero triangle budget exceeded'
    for asset,radius in [('tree',.55),('palm',.48)]:
        _,_,_,r,_,_=compile_one(pathlib.Path('assets/source/'+asset+'.gltf'))
        assert abs(r-radius)<1e-6, 'vegetation collision contract changed'
    print('Hero 5.0.5 geometry, finite normals, all 32 assets, wheel openings: PASS')
if __name__=='__main__':main()
