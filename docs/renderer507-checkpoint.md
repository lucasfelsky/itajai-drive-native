# Renderer 5.0.7 — surveyed Centro landmarks

5.0.6 reserved footprints for eight buildings, but every center, orientation and
extent was an inherited approximation: all eight entries had a facade yaw within
6 degrees of north while the real buildings sit on streets running 20 to 80
degrees off that axis, the Palacio Marcos Konder center was 22 m from its
polygon, the Imaculada nave ran north-south instead of WNW-ESE, the Mercado
anchor stood on a parking strip 50 m from the building, and the Centreventos
model was a quarter of the real footprint. 5.0.7 re-surveys every entry against
the OpenStreetMap building polygons already present in the local cache and
rebuilds the models in that frame.

## Survey (see docs/landmarks-inventory.md for the full table)

Each registry entry now records the OSM way, the oriented bounding box of the
polygon (center, width, depth, axis) and a confidence level. The facade side
was decided from OSM entrance nodes (Museu `entrance=main`, Imaculada door),
named squares and gardens (Praca Gov. Irineu Bornhausen, Praca Vidal Ramos,
Praca Arno Bauer) and outline features (Matriz apse and tower blocks, Palacio
tower bumps, hotel pilasters). The Mercado Publico was located from its
address (Rua Felix Busso Asseburg 25, confluence of Victor Konder, Olimpio
Miranda Jr. and Paulo Bauer); the polygon carries no name, so it is marked
confidence 1. The named OSM polygon 50 m south is the 1980 Centro de
Abastecimento / Mercado do Peixe and stays generic.

| Building | 5.0.6 (w x d, yaw) | 5.0.7 (w x d, yaw) | Center shift |
|----------|--------------------|--------------------|--------------|
| Matriz | 20 x 44, -0.10 | 32.6 x 60.9, 0.679 | 15 m |
| Mercado Publico | 26 x 23, 0.04 | 21.3 x 39.0, -2.985 | 47 m |
| Palacio Marcos Konder | 26 x 17, -0.03 | 21.0 x 51.2, 0.410 | 22 m |
| Casa Konder | 16 x 10, 0.08 | 14.5 x 15.7, 1.773 | 2 m |
| Casa Malburg | 18 x 11, 0.04 | 15.9 x 16.9, 2.301 | 1 m |
| Imaculada Conceicao | 14 x 24, 0.03 | 14.2 x 28.2, -1.243 | 4 m |
| Centreventos | 49 x 31, -0.03 | 94.9 x 127.1, 1.745 | 3 m |
| Pier Turistico | 32 x 13, -0.03 | unchanged (no polygon) | 0 |
| Hotel Rota do Mar | — | 70.8 x 14.0, 0.209 (new) | — |

## Models

All nine buildings are drawn by `src/render/world507_landmarks.inc` in one
local frame per entry (center, facade yaw, local -Z = facade). The landmark
code that was spread over city340, city370, world503, world504 and world505
was removed, so no building is drawn twice. Shared primitives in
`src/render/architecture507.inc`: hip roof, drum, cone, apse, segmental vault,
curved wall. Masses are visible to 1.0–1.6 km; openings, trim and context
props are added inside 380 m.

- Matriz: nave, transept, choir with side chapels, chancel, semicircular apse,
  two 27 m tower blocks with spires flanking a recessed central bay, four stair
  turrets from the outline, three portals, rose, clock, lateral arches and
  buttresses, forecourt fountain, trees and benches.
- Mercado Publico: taller south pavilion with central arch, stepped parapet and
  sign, three wings around an open courtyard with fountain, arcaded flanks,
  sidewalk tables kept off the asphalt, square with palms.
- Palacio Marcos Konder: main block on a high basement, link, rear wing and
  annex following the polygon, three round towers (two on the Hercilio Luz
  facade, one on the Marcos Konder flank), two floors of arched bays with
  pilasters, portal stair, cornices, lamps and garden strip.
- Casa Konder: single storey on basement, steep hip roof with dormer, quoins,
  shuttered windows, entrance stair.
- Casa Malburg: three storeys with belt courses and corner pilasters, arched
  ground floor, balcony, dark steep roof with dormers.
- Imaculada Conceicao: nave with narrower chancel, single front tower with
  belfry and cross, gable face, arched flank windows, paved Praca Vidal Ramos
  with marker, trees and benches.
- Centreventos: full 127 x 95 m footprint with the curved glazed front, side
  wings and service notch, segmental vault with ribs, entrance steps, sign,
  forecourt palms, flag poles and lamps.
- Hotel Rota do Mar (new, historic block beside Casa Konder): two-storey
  arcaded block, set-back west block meeting the Casa wall, east annex.

## Ownership and collision

The replacement predicate is now relative: a cadastral building is replaced
only when the clipped intersection exceeds 0.25 m2 AND at least 30% of the
smaller of the two areas, and when several entries qualify the largest overlap
wins. In 5.0.6 the 2.5 m overlap between the Casa Konder and Hotel Rota do Mar
polygons deleted the entire 70 m hotel; now each polygon goes to its own entry.
Missing sidecar data still falls back to the building bounds with the same
rule. Collision has 25 oriented components matching the new masses; the
Mercado courtyard, the Matriz forecourt and the Centreventos service notch stay
open. `tools/validate_landmarks.py` compiles the production code with MSVC and
runs 134 checks (was 52), including the large-neighbor and adjacency cases.

## Validation

- MSVC production and QA builds pass; clang-cl + lld-link (the CI toolchain)
  builds the same source without errors.
- Vehicle and hero asset validators pass: 32 assets, 8 vehicles, release
  identity 5.0.7.
- Ownership audit on the local 17,305-building cache: 8 replacements, one per
  authored building that has a cadastral polygon, 0 surviving old colliders,
  25 components. No neighbor is deleted.
- Native framebuffer captures at 1584 x 845 with HDR: nine facades, three rear
  views, two night views, an oblique and a distant view at 59–75 FPS
  (5.0.6 captures: 65–76 FPS at the same resolution). The heaviest view is
  the Matriz rear with the Museu in frame.
- Continuous traversal: the QA camera moves at 50 km/h along a 1.69 km
  polyline joining the forecourts of the nine buildings (Praca Vidal Ramos ->
  Casa Malburg -> Casa Konder -> hotel -> Mercado -> Centreventos -> Museu ->
  Matriz) with streaming active: 8,481 frames in 120 s, 70.4 FPS average,
  23.6 FPS worst frame, 34 frames (0.4%) below 50 FPS. This measures streaming
  and draw load along the stretch; it is not a vehicle-physics drive and the
  segments cross blocks.

Compile a separate executable with `ITAJAI_VISUAL_QA` and `ITAJAI_LANDMARK_QA`
to reproduce: it writes `qa507-N.ppm`, `qa507-route-N.ppm`, per-scene metrics,
`qa507-route.txt` and `qa507-ownership.txt`, then exits. Neither define is set
in public builds.

## Known limitations

- Heights, roof profiles, colors and ornament are estimates from public
  descriptions and photographs; nothing is measured on site. Tower tops of the
  Palacio are cones, not domes. The Centreventos vault is a guess at the roof.
- Mercado Publico identification rests on address and position; the north
  chamfer and the real wing heights are not modelled.
- Casa Malburg's facade street and Casa Konder's floor count are inferred.
- The Pier Turistico pavilion is still the 5.0.6 approximation.
- Generic neighbors keep box massing; sidewalks and lots around the authored
  buildings are not yet re-cut, so some facades meet lawn instead of pavement.
- Interiors are not modelled; doors are not traversable.

## References

- OpenStreetMap contributors (ODbL), cached Overpass response and live
  Overpass queries for squares, entrances and the Ferry Boat / pier nodes.
- Prefeitura de Itajai / iPatrimonio, Antigo Mercado Publico:
  https://www.ipatrimonio.org/itajai-antigo-mercado-publico/
- iPatrimonio, Palacio Marcos Konder (three towers, round arches, high
  basement): https://www.ipatrimonio.org/itajai-palacio-marcos-konder/
- Prefeitura de Itajai, Casa Konder (1887, Rua Lauro Muller 83):
  https://visite.itajai.sc.gov.br/casa-konder/
- iPatrimonio, Casa Malburg (1912–1915, three storeys):
  https://www.ipatrimonio.org/itajai-casa-malburg/
- iPatrimonio, Igreja da Imaculada Conceicao (1837–1840):
  https://www.ipatrimonio.org/itajai-igreja-da-imaculada-conceicao/
- Matriz facade reference inherited from 5.0.6.

Geometry is original code; no photograph, map tile or proprietary model is
embedded or redistributed.
