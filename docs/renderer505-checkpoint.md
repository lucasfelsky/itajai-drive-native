# Renderer 5.0.5 — Centro and Novo Uno checkpoint

## Content

- Central cadastral facades: cached street-facing edge selection, secondary long facades at close range, eight muted palettes, storefront glazing, door frames/handles, awnings, signs, cornices, AC units and tower balconies. The pass replaces the overlapping 3.4/3.5/5.0.4 central decorations.
- Hercilio Luz, Felipe Schmidt, Lauro Muller, Marcos Konder and Victor Konder receive deterministic sidewalk/furniture compositions. Waterfront segments gain railing and promenade context; Centreventos gains roof ribs, entrance divisions and steps. Existing landmark bodies are retained.
- Marina craft have tapered hulls, cabin glazing, varied lengths, motorboat roofs or sailing masts. Urban trees and palms have original crown/frond geometry, preserving collider radii.
- Novo Uno has an independent original body generator: short bonnet, tall crowned roof, separate front/rear side glazing, A/B/C posts, actual wheel openings, arch cladding and supported roof rails. Dedicated five split-spoke wheels, rounded headlamp lenses, three asymmetric front openings, tall rear lamps and correctly located wiper replace overlapping generic trim.
- Crosswalk stripes run in the correct direction and nearby duplicates are suppressed.

## Boundaries and performance

No physics, handling, reverse, camera input, networking or updater implementation changes. Central decoration is visual-only. Box geometry is bounded by 1,800 boxes per frame and submitted in one batch; building candidates are sorted by distance (64 maximum), small detail is limited to 105 m, intermediate detail to 230 m and silhouettes to 420 m. Text has a separate ten-label cap.

Local MSVC native build passed. Native framebuffer captures at 1584 x 845, HDR enabled, all 32 assets loaded: four stationary street/vehicle scenes measured approximately 75 FPS after warmup. This is a short fixed-camera smoke test, not a guarantee of 60 FPS throughout a driving session. Scene draw counts were 3,526–3,920. The local baseline compiler emits the existing C4477 debug-format and LNK4210 CRT warnings.

Vehicle gate and hero gate check all eight vehicles, 32 runtime names, index/bounds/finite-normal validity, original vegetation colliders, body triangle budget and actual wheel openings. Public release is conditional on GitHub Build Gate and beta candidate validation.

This is original procedural art informed by Itajai; it is not a surveyed reconstruction or photogrammetry, and does not claim GTA/Forza fidelity. Additional landmark proportion work, more authored blocks, shadow quality and material work remain necessary for that longer-term target.

## Repeatable visual QA

Compile `src/native.c` with `ITAJAI_VISUAL_QA` defined into a separate QA executable, beside the normal map caches and generated PAK. It renders four fixed street/car scenes after warmup, writes `qa505-0.ppm` through `qa505-3.ppm` and matching metrics files, then exits. This code is compiled out of public builds. It does not change the normal camera or input path.

## Public reference notes

Geometry is original; no proprietary imagery/data is embedded.

- Existing cached road and cadastral coordinates: OpenStreetMap (ODbL) and GeoItajai, inherited from 5.0.4.
- Itajai municipality, pedestrian/commercial use of Hercilio Luz: https://itajai.sc.gov.br/noticias/36376/codetran-intensifica-fiscalizacao-de-autopropelidos-e-reforca-seguranca-no-transito
- Municipality, craft-market activity on Hercilio Luz and Beira-Rio: https://itajai.sc.gov.br/noticias/28894/feira-de-artesanato-e-produtos-coloniais-ganha-destaque-na-37-festa-nacional-do-colono
- Fiat, 2014 model-year overview and Way-derived roof bars: https://www.media.stellantis.com/br-pt/fiat/press/linha-2014-do-fiat-novo-uno-traz-novidades-e-uma-nova-serie-especial
- Fiat, original Novo Uno family and Way specification context: https://www.media.stellantis.com/br-pt/fiat/press/novo-uno-novo-tudo
