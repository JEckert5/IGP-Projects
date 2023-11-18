using UnityEngine;

public class Laser : MonoBehaviour {

    private Vector3 mTail;
    private Vector3 mHead;
    private Vector3 mTarget;
    private Vector3 mOrigin;
    private float mSmoothTime;
    private float mMagnitude;
    private Material mMat;

    private static readonly int OriginPosition = Shader.PropertyToID("_OriginPosition");

    private void Awake() {
        mMat = GetComponentInChildren<Renderer>().material;
    }

    // Update is called once per frame
    private void Update() {
        if ((mTarget - transform.position).sqrMagnitude <= 0.05f * 0.05f) { // It's about right
            DestroyImmediate(gameObject);
            return;
        }

        transform.position = Vector3.MoveTowards(transform.position, mTarget, 1 * Time.deltaTime);
    }

    public void SetTarget(Vector3 target, Vector3 origin) {
        mOrigin     = origin;
        mTarget     = target;
        mMagnitude  = (mTarget - mOrigin).magnitude;
        mSmoothTime = mMagnitude / 200; 
        mMat.SetVector(OriginPosition, mOrigin);
        transform.LookAt(mTarget);
        
        Destroy(gameObject, mSmoothTime);
    }
}
