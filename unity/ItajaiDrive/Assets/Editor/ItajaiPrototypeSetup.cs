using ItajaiDrive.CameraSystem;
using ItajaiDrive.Vehicle;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEngine;
using UnityEngine.SceneManagement;

namespace ItajaiDrive.EditorTools
{
    public static class ItajaiPrototypeSetup
    {
        private const string SceneFolder = "Assets/Scenes";
        private const string ScenePath = SceneFolder + "/CentroPrototype.unity";

        [MenuItem("Itajai Drive/Create Centro Prototype Scene")]
        public static void CreateCentroPrototype()
        {
            if (!AssetDatabase.IsValidFolder(SceneFolder))
                AssetDatabase.CreateFolder("Assets", "Scenes");

            Scene scene = EditorSceneManager.NewScene(NewSceneSetup.EmptyScene, NewSceneMode.Single);
            scene.name = "CentroPrototype";

            var environment = new GameObject("Environment");

            var ground = GameObject.CreatePrimitive(PrimitiveType.Plane);
            ground.name = "Centro Ground Placeholder";
            ground.transform.SetParent(environment.transform);
            ground.transform.position = Vector3.zero;
            ground.transform.localScale = new Vector3(60f, 1f, 60f);

            var sunObject = new GameObject("Sun");
            sunObject.transform.SetParent(environment.transform);
            sunObject.transform.rotation = Quaternion.Euler(42f, -32f, 0f);
            var sun = sunObject.AddComponent<Light>();
            sun.type = LightType.Directional;
            sun.intensity = 1.1f;
            sun.shadows = LightShadows.Soft;

            var player = GameObject.CreatePrimitive(PrimitiveType.Cube);
            player.name = "Player - Novo Uno Migration Placeholder";
            player.transform.position = new Vector3(0f, 0.62f, 0f);
            player.transform.localScale = new Vector3(1.64f, 0.82f, 3.81f);
            var rigidbody = player.AddComponent<Rigidbody>();
            rigidbody.mass = 1120f;
            rigidbody.linearDamping = 0.04f;
            rigidbody.angularDamping = 0.9f;
            player.AddComponent<ArcadeVehicleController>();

            var cameraObject = new GameObject("Main Camera");
            cameraObject.tag = "MainCamera";
            cameraObject.transform.position = new Vector3(0f, 2.7f, -6.2f);
            cameraObject.AddComponent<UnityEngine.Camera>().fieldOfView = 67f;
            cameraObject.AddComponent<AudioListener>();
            var chase = cameraObject.AddComponent<ChaseCamera>();
            chase.SetTarget(player.transform);

            var marker = new GameObject("MIGRATION NOTE - replace placeholders with Centro data and Uno hero");
            marker.transform.SetParent(environment.transform);

            EditorSceneManager.SaveScene(scene, ScenePath);
            Selection.activeGameObject = player;
            AssetDatabase.SaveAssets();
            Debug.Log("ITAJAÍ DRIVE: CentroPrototype created. Next migration step is road/cadastral import + Novo Uno hero prefab.");
        }
    }
}
