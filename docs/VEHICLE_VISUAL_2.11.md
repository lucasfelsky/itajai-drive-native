# 2.11.0 — Vehicle Visual 2.0 + Smart Chase Camera

This checkpoint closes the named vehicle-part contract introduced in 2.10 and folds in the first real QA-driven driving-camera fixes.

## Vehicle presentation

- Each of the eight vehicle families resolves an independent `body`, `glass`, and `trim` mesh from the runtime PAK.
- Glass uses the dedicated glass material instead of being painted as bodywork.
- Trim is drawn separately with rubber/plastic response.
- The same part contract is used by the player and nearby traffic; procedural wheels/lights remain as animated fallback/details.
- The final render loop now explicitly draws the named vehicle parts after the legacy vehicle body pass.

## Reverse steering

Keyboard steering is made intuitive while reversing: `A` means reverse-left and `D` means reverse-right from the driver's/game-camera perspective. The underlying vehicle yaw math remains unchanged; the input sign is adapted only while the reverse state is active.

## Smart chase camera

Third-person chase cameras now have a canonical rear-facing state.

- Mouse movement temporarily owns the orbit camera.
- After roughly 0.9 seconds of mouse inactivity the camera eases back to the canonical rear view.
- Reverse motion automatically rotates the chase camera by 180 degrees to show the direction of travel.
- Returning to forward motion restores the normal chase view immediately.
- After stopping from reverse, the reverse-facing camera remains for roughly five seconds before returning.
- Cockpit and overhead/free cameras remain manual.

## Compatibility

The existing OpenGL compatibility path and the 2.10 asset PAK format are retained. Missing named parts simply fall back to the procedural presentation instead of preventing startup.
