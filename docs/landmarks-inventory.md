# Centro landmark inventory (versioned)

Registry: `src/world/landmarks507.inc`. Models: `src/render/world507_landmarks.inc`.
Coordinates and oriented extents come from the OpenStreetMap building polygons
cached in `itajai_osm_cache_v13.json` (ODbL). Facade side is derived from OSM
entrance nodes, squares and adjacent streets. Every height, roof profile and
material is an estimate from public descriptions unless stated otherwise.
Confidence: 2 = polygon + entrance/square evidence, 1 = polygon only (facade
inferred from streets), 0 = no cadastral polygon.

| # | Building | OSM way | Center (lat, lon) | Footprint w x d (m) | Facade yaw (rad) / faces | Conf | Model state | Pending |
|---|----------|---------|-------------------|---------------------|--------------------------|------|-------------|---------|
| 0 | Igreja Matriz do Santissimo Sacramento | w857401220 (62 vertices, height tag 12) | -26.9075840, -48.6618468 | 32.6 x 60.9 (polygon 1542 m2) | 0.679 / NNW, Praca Gov. Irineu Bornhausen | 2 | Nave, transept, choir, chancel, semicircular apse, two tower blocks flanking a recessed central bay (towers 27 m + spire 9.5 m), four stair turrets from the outline, three portals, rose, clock, lateral arches with buttresses, forecourt with fountain | Tower height and spire profile unverified; facade ornament simplified; service lane on the -X flank not modelled |
| 1 | Antigo Mercado Publico (Mercado Velho, 1917 / 1936) | w298841666 (unnamed; Rua Felix Busso Asseburg 25, between the street, Av. Prefeito Paulo Bauer and Rua Olimpio Miranda Jr.) | -26.9060089, -48.6544588 | 21.3 x 39.0 | -2.985 / S toward Praca Felix Busso Asseburg | 1 | Taller south pavilion with central arch and stepped parapet, three wings around an open courtyard with fountain, arcaded flanks, sign, sidewalk tables, square with palms | Identification by address/position only (the named OSM polygon 50 m south is the 1980 Centro de Abastecimento / Mercado do Peixe, left generic); north chamfer not modelled; wing heights estimated |
| 2 | Palacio Marcos Konder / Museu Historico | w1097529372 (36 vertices) | -26.9071056, -48.6612274 | 21.0 x 51.2 | 0.410 / NNW toward Rua Hercilio Luz (OSM entrance=main) | 2 | Main block on high basement, link, rear wing and annex following the outline, three round towers (two on the facade, one on the Av. Marcos Konder flank), arched bays on two floors, pilasters, cornices, portal stair, sign, lamps | Tower roofs are cones (real tops are domed/pyramidal); rear wing may partly be garden in reality; colors from photographs, not sampled |
| 3 | Casa Konder (1887) | w1098072184 | -26.9057948, -48.6553532 | 14.5 x 15.7 | 1.773 / W toward Rua Lauro Muller | 1 | Single storey on basement, steep hip roof with dormer and chimney, quoins, framed windows with shutters, entrance stair | OSM says one level; several sources describe a larger manor, floor count unverified |
| 4 | Casa Malburg (1912-1915) | w1106146084 | -26.9043835, -48.6561088 | 15.9 x 16.9 | 2.301 / SW toward Rua Pedro Ferreira | 1 | Three storeys with belt courses, corner pilasters, arched ground floor, balcony, dark steep roof with dormers and chimneys | Facade street inferred (Pedro Ferreira is the only street within 20 m); roof form (mansard vs hip) unverified |
| 5 | Igreja da Imaculada Conceicao ("Igrejinha Velha", 1837-1840) | w681517239 | -26.9050623, -48.6561982 | 14.2 x 28.2 | -1.243 / ESE toward Praca Vidal Ramos (OSM entrance node) | 2 | Nave, narrower chancel, single front tower with belfry and cross, gable face, arched flank windows, forecourt square with Marco Zero marker, trees and benches | Tower proportions estimated; interior art not represented |
| 6 | Centreventos Itajai | w197483217 (19 vertices) | -26.9096087, -48.6528916 | 94.9 x 127.1 (polygon 9671 m2) | 1.745 / W toward Av. Ministro Victor Konder | 1 | Hall footprint from the outline including the curved glazed front, side wings, river-side service notch; segmental vault with ribs, entrance steps, forecourt palms, flag poles, lamps | Roof shape is an estimate; facade colour from older material; the 5.0.6 model was 49 x 31 m |
| 7 | Pier Turistico (Pier Guilherme Asseburg) pavilion | w681517238 is a pier, no building polygon | -26.904722, -48.654444 | 32 x 13 | -0.03 / N | 0 | 5.0.6 pavilion kept in the shared frame | Needs survey; sign text inherited |
| 8 | Hotel Rota do Mar (historic block beside Casa Konder) | w1106146082 (30 vertices) | -26.9058694, -48.6550575 | 70.8 x 14.0 | 0.209 / N toward the river | 1 | Two-storey main block, set-back west block meeting Casa Konder, single-storey east annex, arcaded ground floor with pilasters and balconies, hip roofs | OSM polygon overlaps Casa Konder by about 2.5 m; the model west block is pulled back to the Casa wall instead |

## Neighbors verified around the stretch (left generic on purpose)

- Ki Pastel (w857401227) and the 3-storey retail block (w857401226) east of the Matriz.
- Receita Federal (w1222391922) and Hotel Valerim (w1222391923) around Casa Malburg / Praca Vidal Ramos.
- Centro de Abastecimento Prefeito Paulo Bauer / Mercado do Peixe (w298841660), south of the Mercado Publico.
- Brazero Garcia restaurant at the Marina (w1056874355).

## Survey method

1. Extract the building polygon from the cached Overpass response and compute
   its minimum-area oriented rectangle (center, extents, axis).
2. Decide the facade side from: OSM `entrance` nodes, named squares/gardens
   (`leisure=park|garden`), outline features (apse, tower bumps, pilasters) and
   the nearest named streets.
3. Convert to the model frame: local -Z is the facade; yaw = atan2(-nx, -nz)
   of the facade normal in (east, south) coordinates.
4. Register footprint and collision components, then compare the ownership
   audit (`qa507-ownership.txt`) against the neighbor list above.
