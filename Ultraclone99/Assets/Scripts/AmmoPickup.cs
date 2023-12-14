using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AmmoPickup : MonoBehaviour {
    [SerializeField] private int ammo;

    private Transform mPlayer;

    private float mYPos;
    private float mVariance;
    private bool mDir;
    private float mYVel;

    private void Start() {
        mPlayer = GameObject.FindGameObjectWithTag("Player").transform;
        mYPos   = transform.position.y;

        mVariance = mYPos + 1f;
    }

    private void Update() {
        transform.LookAt(mPlayer);
        transform.forward = -transform.forward;
        
        mYPos = mDir ? Mathf.SmoothDamp(mYPos, mVariance, ref mYVel, 0.4f) : Mathf.SmoothDamp(mYPos, -mVariance, ref mYVel, 0.4f);
        
        var position = transform.position;
        position           = new Vector3(position.x, mYPos, position.z);
        transform.position = position;

        mDir = !(mYPos >= mVariance - 0.1f);
    }

    private void OnTriggerEnter(Collider other) {
        if (!other.gameObject.CompareTag("Player")) return;

        var p = other.gameObject.GetComponent<PlayerController>();
        
        p.AddAmmo(ammo);

        Destroy(gameObject);
    }
}
