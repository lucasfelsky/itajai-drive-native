# Renderer 5.0.6 — individual landmark ownership

Generic cadastral volumes previously remained inside manually drawn landmarks.
The same buildings could also emit storefronts, rooftop equipment, night windows,
shadows and collision after their visible replacement was added.

## Content and replacement

Eight named entries now reserve the footprints of Matriz, Mercado Publico,
Palacio Marcos Konder, Casa Konder, Casa Malburg, Imaculada Conceicao,
Centreventos and the passenger terminal. Exact municipal polygons are clipped
against each oriented model footprint. Shared edges do not count as overlap;
positive intersections above 0.25 square metres replace the cadastral building.
The decision is independent of draw distance, quality and stream position.
Missing polygon data uses the building bounds. A neighboring building whose
LOD box would intrude retains its exact geometry instead of being deleted.

All generic volume paths and secondary facade/roof/light/shadow passes share
the replacement predicate. The source caches and roads are not rewritten.
Static collision registration skips replaced buildings and adds 16 oriented
occupied components matching the authored masses. The market courtyard is empty.
The narrow phase transforms points and normals; driving equations are unchanged.

Five buildings received additional original geometry:

- Matriz: three entrance arches, rose tracery, clock, nave roof, lateral windows,
  buttresses, tower openings and cornices.
- Museu / Palacio Marcos Konder: arched windows on front and side returns,
  projecting tower windows and corner trim.
- Mercado: four separate pitched roof wings and arched green openings.
- Casa Konder: seated full-width roof, framed openings, entrance and side windows.
- Casa Malburg: two-storey window rhythm, entrance, roof, belt courses and returns.

Historic plaster no longer receives the generic skyscraper window texture.
Cafe tables are omitted when their placement would be on an asphalt road.

## Validation

`python tools/validate_landmarks.py` compiles production clipping and collision
code with MSVC and exercises 52 cases: rotated overlap, containing polygons,
crossing edges, touching/separated neighbors, exact-vs-AABB decisions, absent
sidecar, occupied wall contacts, empty courtyard/corners and world-space normals.
This gate runs in the Renderer Build Gate workflow.

Local production and QA MSVC builds pass. Existing C4477 debug-format and LNK4210
CRT warnings remain. The hero asset validator passes with all 32 assets and 8 cars.

On the local 17,305-building municipal cache, the ownership audit finds seven
replacements, zero surviving old colliders and all 16 authored components.
Not every protected lot has a conflicting source building in this cache.
Twelve native framebuffer scenes cover all eight entries plus rear, night and
distant views. Short stationary captures at 1584 x 845 with HDR and 32 assets
are approximately 65–76 FPS; this is not a full-route driving benchmark.

For repeatable captures compile a separate executable with both
`ITAJAI_VISUAL_QA` and `ITAJAI_LANDMARK_QA`. It writes `qa506-*.ppm`, frame metrics
and `qa506-ownership.txt`, then exits. Neither QA mode is enabled in public builds.

## Continuing building by building

The named footprint registry is the starting point for incremental replacement,
not a claim that the entire city has been reconstructed. Coordinates and masses
are inherited approximations; roof profiles, colors and proportions still need
individual survey/reference refinement. In particular the simplified Centreventos
mass is smaller than the municipal source footprint it replaces. Interiors are
not modeled, and facade doors do not imply traversable indoor space.

Add each complete building to the named registry, define its occupied components,
then verify its neighbors and four sides in QA before releasing. Avoid registering
an incomplete facade-only decoration as a replacement for a whole building.

Reference context: existing OSM/GeoItajai cache and public photographs of the
Matriz facade (three portals, rose window, central clock and twin spires), including
https://www.behance.net/gallery/169701917/Igreja-Matriz-de-Itajai . Geometry is
original code; no source photograph is embedded or redistributed in the game.
