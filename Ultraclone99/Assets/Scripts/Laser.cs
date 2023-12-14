using System;
using UnityEngine;

public class Laser : MonoBehaviour {

    private Vector3 mTail;
    private Vector3 mHead;
    private Vector3 mTarget;
    private Vector3 mOrigin;
    private Vector3 mFirstScale;
    private float mTiterator = 0f;
    private Material mMat;
    private float mYScale;

    private readonly static int OriginPosition = Shader.PropertyToID("_OriginPosition");

    private void Awake() {
        mMat = GetComponentInChildren<Renderer>().material;
    }

    // Update is called once per frame
    private void Update() {
        var x = Mathf.Lerp(mFirstScale.x, 0, mTiterator);
        var z = Mathf.Lerp(mFirstScale.z, 0, mTiterator);
        transform.localScale = new Vector3(x, mYScale, z);

        mTiterator += Time.deltaTime * 6f;
    }

    public void SetTarget(Vector3 target, Vector3 origin) {
        mOrigin     = origin;
        mTarget     = target;
        mMat.SetVector(OriginPosition, mOrigin);
        transform.LookAt(mTarget);
        transform.Rotate(Vector3.right, 90);

        var mag = (mTarget - mOrigin).magnitude / 2f;
        transform.position = mOrigin + (mTarget - mOrigin) / 2;
        mFirstScale        = transform.localScale;

        mYScale = transform.localScale.y * mag;

        transform.localScale = new Vector3(mFirstScale.x, mYScale, mFirstScale.z);
        Destroy(gameObject, 1.5f);
    }
}
