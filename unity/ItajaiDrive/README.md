# ITAJAÍ DRIVE — Unity client

This folder is the new Unity client. Open `unity/ItajaiDrive` as a project in Unity 6.3 LTS.

Initial packages are HDRP, Input System and Cinemachine. On the first editor open, let Package Manager resolve dependencies. If HDRP is not yet the active render pipeline, use the HDRP Project Wizard once to create/assign the pipeline assets; those generated project assets should then be committed to this branch.

After scripts compile, run `Itajai Drive > Create Centro Prototype Scene`. The generated scene is only a migration smoke test: a metric world, Rigidbody placeholder vehicle and chase/reverse camera. It is not intended as final gameplay or final art.

Next work items are the Centro road/building importer, eight landmark prefabs from the 5.0.6 ownership registry, and the Novo Uno hero prefab/material hierarchy.
