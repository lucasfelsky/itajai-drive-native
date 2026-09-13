using UnityEngine;
#if ENABLE_INPUT_SYSTEM
using UnityEngine.InputSystem;
#endif

namespace ItajaiDrive.Vehicle
{
    [RequireComponent(typeof(Rigidbody))]
    public sealed class ArcadeVehicleController : MonoBehaviour
    {
        [Header("Reference handling baseline")]
        [SerializeField] private float engineAcceleration = 11.5f;
        [SerializeField] private float reverseAcceleration = 7.5f;
        [SerializeField] private float maxForwardSpeed = 44.5f;
        [SerializeField] private float maxReverseSpeed = 12.0f;
        [SerializeField] private float steeringTorque = 5.2f;
        [SerializeField] private float steeringFadeSpeed = 31.0f;
        [SerializeField] private float lateralGrip = 8.5f;
        [SerializeField] private float rollingDrag = 0.25f;
        [SerializeField] private float brakeStrength = 18.0f;
        [SerializeField] private float handbrakeGripMultiplier = 0.22f;
        [SerializeField] private Vector3 centerOfMassOffset = new(0f, -0.42f, 0.05f);

        private Rigidbody body;
        private float throttle;
        private float steer;
        private bool brake;
        private bool handbrake;

        public float SpeedKph => body == null ? 0f : Vector3.Dot(body.linearVelocity, transform.forward) * 3.6f;

        private void Awake()
        {
            body = GetComponent<Rigidbody>();
            body.centerOfMass += centerOfMassOffset;
            body.interpolation = RigidbodyInterpolation.Interpolate;
            body.collisionDetectionMode = CollisionDetectionMode.ContinuousDynamic;
        }

        private void Update()
        {
            ReadInput();
        }

        private void FixedUpdate()
        {
            Vector3 velocity = body.linearVelocity;
            float forwardSpeed = Vector3.Dot(velocity, transform.forward);
            float lateralSpeed = Vector3.Dot(velocity, transform.right);

            float accel = throttle >= 0f ? engineAcceleration : reverseAcceleration;
            float speedLimit = throttle >= 0f ? maxForwardSpeed : maxReverseSpeed;
            if (Mathf.Abs(forwardSpeed) < speedLimit || Mathf.Sign(throttle) != Mathf.Sign(forwardSpeed))
                body.AddForce(transform.forward * throttle * accel, ForceMode.Acceleration);

            float speed01 = Mathf.Clamp01(Mathf.Abs(forwardSpeed) / Mathf.Max(1f, steeringFadeSpeed));
            float steerAuthority = Mathf.Lerp(0.45f, 1f, speed01);
            body.AddTorque(Vector3.up * steer * steeringTorque * steerAuthority, ForceMode.Acceleration);

            float grip = lateralGrip * (handbrake ? handbrakeGripMultiplier : 1f);
            body.AddForce(-transform.right * lateralSpeed * grip, ForceMode.Acceleration);

            if (brake)
            {
                Vector3 planar = Vector3.ProjectOnPlane(body.linearVelocity, Vector3.up);
                body.AddForce(-planar * brakeStrength, ForceMode.Acceleration);
            }

            body.AddForce(-body.linearVelocity * rollingDrag, ForceMode.Acceleration);
        }

        private void ReadInput()
        {
#if ENABLE_INPUT_SYSTEM
            float keyboardThrottle = 0f;
            float keyboardSteer = 0f;
            bool keyboardBrake = false;
            bool keyboardHandbrake = false;

            if (Keyboard.current != null)
            {
                keyboardThrottle = (Keyboard.current.wKey.isPressed ? 1f : 0f) - (Keyboard.current.sKey.isPressed ? 1f : 0f);
                keyboardSteer = (Keyboard.current.dKey.isPressed ? 1f : 0f) - (Keyboard.current.aKey.isPressed ? 1f : 0f);
                keyboardBrake = Keyboard.current.leftShiftKey.isPressed || Keyboard.current.rightShiftKey.isPressed;
                keyboardHandbrake = Keyboard.current.spaceKey.isPressed;
            }

            float padThrottle = 0f;
            float padSteer = 0f;
            bool padBrake = false;
            bool padHandbrake = false;
            if (Gamepad.current != null)
            {
                padThrottle = Gamepad.current.rightTrigger.ReadValue() - Gamepad.current.leftTrigger.ReadValue();
                padSteer = Gamepad.current.leftStick.x.ReadValue();
                padBrake = Gamepad.current.buttonWest.isPressed;
                padHandbrake = Gamepad.current.buttonSouth.isPressed;
            }

            throttle = Mathf.Abs(padThrottle) > Mathf.Abs(keyboardThrottle) ? padThrottle : keyboardThrottle;
            steer = Mathf.Abs(padSteer) > Mathf.Abs(keyboardSteer) ? padSteer : keyboardSteer;
            brake = keyboardBrake || padBrake;
            handbrake = keyboardHandbrake || padHandbrake;
#else
            throttle = Input.GetAxisRaw("Vertical");
            steer = Input.GetAxisRaw("Horizontal");
            brake = Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift);
            handbrake = Input.GetKey(KeyCode.Space);
#endif
        }
    }
}
