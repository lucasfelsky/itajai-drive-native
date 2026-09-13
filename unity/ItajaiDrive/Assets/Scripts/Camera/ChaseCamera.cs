using UnityEngine;

namespace ItajaiDrive.CameraSystem
{
    public sealed class ChaseCamera : MonoBehaviour
    {
        [SerializeField] private Transform target;
        [SerializeField] private Vector3 localOffset = new(0f, 2.25f, -5.8f);
        [SerializeField] private float positionSharpness = 8f;
        [SerializeField] private float rotationSharpness = 10f;
        [SerializeField] private float lookHeight = 0.75f;
        [SerializeField] private float reverseSpeedThreshold = -1.2f;

        private Rigidbody targetBody;

        public void SetTarget(Transform value)
        {
            target = value;
            targetBody = target == null ? null : target.GetComponent<Rigidbody>();
        }

        private void Awake()
        {
            if (target != null)
                targetBody = target.GetComponent<Rigidbody>();
        }

        private void LateUpdate()
        {
            if (target == null)
                return;

            float forwardSpeed = targetBody == null ? 0f : Vector3.Dot(targetBody.linearVelocity, target.forward);
            bool reversing = forwardSpeed < reverseSpeedThreshold;
            Vector3 offset = localOffset;
            if (reversing)
                offset.z = Mathf.Abs(localOffset.z);

            Vector3 desired = target.TransformPoint(offset);
            float posT = 1f - Mathf.Exp(-positionSharpness * Time.deltaTime);
            transform.position = Vector3.Lerp(transform.position, desired, posT);

            Vector3 lookPoint = target.position + Vector3.up * lookHeight;
            Quaternion desiredRotation = Quaternion.LookRotation(lookPoint - transform.position, Vector3.up);
            float rotT = 1f - Mathf.Exp(-rotationSharpness * Time.deltaTime);
            transform.rotation = Quaternion.Slerp(transform.rotation, desiredRotation, rotT);
        }
    }
}
