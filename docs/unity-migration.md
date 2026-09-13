# ITAJAÍ DRIVE — Unity migration

## Decision

The native Win32/OpenGL renderer is frozen as the 5.0.6 reference implementation. New gameplay/world development moves to Unity on branch `dev/unity63-hdrp`.

Target editor: Unity 6.3 LTS. Target render pipeline: HDRP. Primary platform: Windows desktop.

The native implementation is not deleted. It remains the source of truth for validated behavior and useful data until each subsystem is replaced and runtime-validated in Unity.

## What we keep

- OpenStreetMap / GeoItajaí-derived road and cadastral data.
- Landmark coordinates and authored footprint ownership from Renderer 5.0.6.
- Vehicle dimensions and the Novo Uno hero reference/generator as migration input.
- Existing handling/camera behavior as a reference to reproduce, not as code that must be mechanically translated.
- The current stable/beta updater history and releases remain untouched while the Unity client is immature.

## What we replace

- Win32/OpenGL renderer -> Unity HDRP.
- Immediate-mode/procedural drawing -> meshes, prefabs, materials, GPU instancing and authored scene content.
- Native render loop -> Unity scenes and gameplay systems.
- Native input/window code -> Unity Input System.
- Custom world streaming -> Unity scene/chunk streaming and Addressables when the prototype is stable.
- Vehicle presentation -> Unity mesh/prefab hierarchy with PBR materials.

## Rendering direction

The project targets a high-fidelity coastal Brazilian city rather than the old procedural look. Use HDRP PBR materials, physically plausible lighting, volumetric atmosphere, reflection/probe systems, proper LODs, decals, wet-surface response and dense but deterministic street dressing.

The first visual target is the central Itajaí corridor: Mercado Público -> Matriz -> Hercílio Luz -> Museu/Palácio Marcos Konder -> Beira-Rio -> Centreventos/Marina.

## First playable vertical slice

1. Create an HDRP Unity project that boots directly into a Centro test scene.
2. Establish a local metric coordinate system centered on Itajaí and import the existing road/building data without using raw latitude/longitude as Unity world coordinates.
3. Generate/import the main roads and cadastral masses for the Centro.
4. Rebuild the eight 5.0.6 owned landmarks as proper Unity prefabs so generic cadastral buildings do not overlap them.
5. Import/rebuild the Novo Uno Way as the first hero vehicle.
6. Implement a drivable Rigidbody-based vehicle while matching the validated native feel before adding simulation complexity.
7. Add third-person chase/reverse camera behavior and gamepad/keyboard input.
8. Establish HDRP day lighting, sky, fog, reflections and baseline PBR materials.
9. Add LOD/streaming/performance instrumentation before increasing city density.

## Migration rules

- Do not delete or rewrite the 5.0.6 native branch during the migration.
- Do not port multiplayer yet.
- Do not attempt a mechanical line-by-line C-to-C# conversion of the renderer.
- Preserve geographic/data work; rebuild rendering/gameplay around Unity-native systems.
- Prefer proper meshes/materials/prefabs over runtime piles of primitive boxes.
- Keep the Centro as the quality bar before expanding to the rest of Itajaí.
- Every migrated subsystem must be tested against the old runtime behavior where a validated reference exists.

## Versioning

The Unity branch starts a new product line. Do not continue the native `5.0.x` renderer version sequence for Unity builds. Use a separate Unity prototype version line until the migration is ready to replace the native beta channel.
