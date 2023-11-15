using UnityEngine;


public class Laser : MonoBehaviour {

    [SerializeField] private float lifetime;
    
    private LineRenderer mLineRenderer;

    private Vector3 mTail;
    private Vector3 mHead;
    private Vector3 mTarget;
    private Vector3 mOrigin;
    private Vector3 mTiterator;
    private Vector3 mTiterator2;
    private float mSmoothTime;

    private void Awake() {
        mLineRenderer = GetComponent<LineRenderer>();
    }

    // Update is called once per frame
    private void Update() {
        if ((mTarget - transform.position).sqrMagnitude <= 0.05f * 0.05f) { // It's about right
            DestroyImmediate(gameObject);
            return;
        }

        transform.position = Vector3.MoveTowards(transform.position, mTarget, mSmoothTime / Time.deltaTime);
        Debug.Log(transform.position);
        var normal = transform.forward;
        mHead += normal * 1.2f;
        mTail += normal * 0.8f;
        mLineRenderer.SetPositions(new []{mTail, mHead});
    }

    public void SetTarget(Vector3 target, Vector3 origin) {
        mOrigin     = origin;
        mTarget     = target;
        mTail       = mOrigin;
        var magnitude  = (mTarget - mOrigin).magnitude;
        mHead       = Vector3.Lerp(mTail, mTarget, 2 / magnitude);
        mSmoothTime = magnitude / 200; // percent of lifetime
        
        // Debug.Log(mSmoothTime);
        
        mLineRenderer.SetPosition(0, mTail);
        mLineRenderer.SetPosition(1, mHead);
        
        Destroy(gameObject, mSmoothTime);
    }
}
