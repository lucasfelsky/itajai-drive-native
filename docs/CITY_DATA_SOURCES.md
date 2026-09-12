# ITAJAÍ DRIVE — City Data Sources

## 3.3 City Rebuild policy

The city pipeline uses open/public geospatial sources only. Google Maps, Google Earth and Street View may be used manually as visual reference, but the runtime/build pipeline does not scrape, download or redistribute proprietary Google geometry, imagery or textures.

## Runtime baseline

### OpenStreetMap / Overpass

OSM remains the authoritative runtime source for the drivable graph in 3.3:

- highway centerlines
- road names and classifications
- lane hints
- one-way direction
- max-speed hints
- traffic signals
- building fallback data

Endpoints currently supported:

- `overpass-api.de`
- `overpass.kumi.systems`

The downloaded OSM response is cached locally and the routing graph remains resident while render geometry is region-streamed.

## Municipal enrichment — GeoItajaí

Public ArcGIS services exposed by Prefeitura de Itajaí are used as municipal enrichment where they materially improve fidelity.

ArcGIS REST root:

- `https://arcgis.itajai.sc.gov.br/server/rest/services`

GeoItajaí portal:

- `https://geo.itajai.sc.gov.br/`

### Buildings — enabled in 3.3

Service:

- `Base_Cadastral_Edificacao/FeatureServer/0`

Useful fields:

- `objectid`
- `nome`
- `tipouso`
- `numpav`

The service provides polygon geometry and supports pagination. ITAJAÍ DRIVE asks the service to return geometry in EPSG:4326, projects it through the same local projection used by the OSM graph, and reuses the existing exact-footprint renderer/collision system.

The municipal building set is accepted only when pagination completes successfully. If the service is unavailable or returns an incomplete dataset, the game keeps the OSM building set instead of committing a partial city.

Municipal building data is cached in:

- the existing binary world cache for the building records
- `itajai_buildings_v330.bin` for exact footprint vertices

This means normal subsequent launches do not require GeoItajaí to be online.

### Candidate municipal layers for later 3.3.x city passes

The following public layers were identified and should be integrated only where they improve the driving scene without creating unnecessary runtime weight:

- `Bairros` — neighborhood identity / zoning rules
- `Sistema_Transporte_Trecho_Arruamento` — cadastral street geometry
- `Sistema_Transporte_Trecho_Rodoviario` — road geometry/reference
- `Energia_e_Comunicacoes_Poste` — real pole placement
- `Ciclovias` / `sistema_cicloviario` — cycle infrastructure
- `Classes_Base_Canteiro_Central` — median geometry
- `Classes_Base_Ponte` — bridges
- cadastral lots / `lotes` services — parcel boundaries and realistic setbacks
- `Relevo_Curva_Nivel` and `Relevo_Ponto_Cotado_Altimetrico` — terrain refinement
- municipal tree/green-area layers — vegetation placement
- `Hosted/edif3d_WSL1` — 3D building reference, subject to format/performance validation before use

## Fallback rules

1. Load validated local binary caches when available.
2. If municipal building cache is missing, attempt GeoItajaí enrichment.
3. Keep OSM roads/signals regardless of municipal building availability.
4. If municipal enrichment fails, keep exact OSM footprints.
5. If OSM network retrieval also fails, use the existing offline fallback world.

No online source is allowed to leave a half-populated world committed to cache.

## Attribution

The loading UI identifies OpenStreetMap and GeoItajaí / Prefeitura de Itajaí as open-data sources. Release documentation should preserve source attribution as additional municipal layers are enabled.
