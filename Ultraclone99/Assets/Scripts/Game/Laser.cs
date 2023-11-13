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
    
    // Start is called before the first frame update
    private void Start() {
    }

    private void Awake() {
        mLineRenderer = GetComponent<LineRenderer>();
    }

    // Update is called once per frame
    private void Update() {
        //mTail = Vector3.Lerp(mTail, mHead, mTiterator);
        //mLineRenderer.SetPosition(0, mTail);
        //mTiterator += Time.deltaTime;
    }

    public void SetTarget(Vector3 target, Vector3 spawn) {
        mLineRenderer.SetPosition(0, spawn);
        mLineRenderer.SetPosition(1, target);
        // Debug.Log("line renderer" + mLineRenderer.GetPosition(1));

        mTail = spawn;
        mHead = target;
        
        Debug.Log(target);
        
        Destroy(gameObject, 1.2f);
    }
}
