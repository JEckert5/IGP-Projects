using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.ProBuilder.MeshOperations;
using UnityEngine.Serialization;

public class Laser : MonoBehaviour {
    
    private LineRenderer mLineRenderer;

    private Vector3 mTail;
    private Vector3 mHead;
    private float mTiterator;
    private float mMagnitude;
    
    // Start is called before the first frame update
    private void Start() {
    }

    private void Awake() {
        mLineRenderer = GetComponent<LineRenderer>();
    }

    // Update is called once per frame
    private void Update() {
        mTail = Vector3.Lerp(mTail, mHead, mTiterator);
        mLineRenderer.SetPosition(0, mTail);

        mTiterator += Time.deltaTime / (mMagnitude / 6);

        System.Math.Sqrt(333);
    }

    public void SetTarget(Vector3 target, Vector3 spawn) {
        mLineRenderer.SetPosition(0, spawn);
        mLineRenderer.SetPosition(1, target);

        mTail = spawn;
        mHead = target;
        
        mMagnitude = (mHead - mTail).sqrMagnitude;
        
        Debug.Log(target);
        
        Destroy(gameObject, 1.2f);
    }
}
