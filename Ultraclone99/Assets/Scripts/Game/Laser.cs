using System;
using UnityEngine;

public class Laser : MonoBehaviour {

    private Vector3 mTail;
    private Vector3 mHead;
    private Vector3 mTarget;
    private Vector3 mOrigin;
    private Material mMat;

    public Rigidbody rb;

    private readonly static int OriginPosition = Shader.PropertyToID("_OriginPosition");

    private void Awake() {
        mMat = GetComponentInChildren<Renderer>().material;
    }

    // Update is called once per frame
    private void Update() {
        // Debug.Log(rb.transform.position);
        
        var t = Vector3.MoveTowards(transform.position, mTarget, 50 * Time.deltaTime);
        
        rb.MovePosition(t * 2);
    }

    public void SetTarget(Vector3 target, Vector3 origin) {
        mOrigin     = origin;
        mTarget     = target;
        mMat.SetVector(OriginPosition, mOrigin);
        transform.LookAt(mTarget);
        
        Destroy(gameObject, 1.5f);
    }

    private void OnCollisionEnter(Collision other) {
        Debug.Log("Help");
    }

    private void OnCollisionStay(Collision other) {
        Debug.Log("Stay");
    }

    private void OnCollisionExit(Collision other) {
        Debug.Log("Leave");
    }
}
