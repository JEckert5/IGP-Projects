using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.ProBuilder.MeshOperations;
using UnityEngine.Serialization;

public class Laser : MonoBehaviour {

    private Transform mTarget;
    private LineRenderer mLineRenderer;
    
    // Start is called before the first frame update
    private void Start() {
    }

    private void Awake() {
        mLineRenderer = GetComponent<LineRenderer>();
    }

    // Update is called once per frame
    private void Update() {
        if (mTarget.Equals(null)) return;
        
        // Retract tail.
    }

    public void SetTarget(Transform target) {
        mTarget = target;
        
        mLineRenderer.SetPosition(0, mTarget.position);
    }
}
