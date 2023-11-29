using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;
using TMPro;
using UnityEngine.SceneManagement;
using UnityEngine.Serialization;

public class GunkyLadController: MonoBehaviour {

    private NavMeshAgent mAgent;
    private Transform mDestination; // Player
    private Wave mWave;
    private Rigidbody mRigidbody;
    private bool mDead;
    private Vector3 mDeathVec; // Direction he flies when dead 
    
    [SerializeField] private Transform healthTextTransform;
    [SerializeField] private int health;
    [SerializeField] private TextMeshProUGUI healthText;
    
    // Start is called before the first frame update
    private void Start() {
        mAgent                      = GetComponent<NavMeshAgent>();
        mRigidbody                  = GetComponent<Rigidbody>();
        mRigidbody.detectCollisions = true;
        var t = GameObject.FindGameObjectWithTag("Player");

        if (t == null) return;

        mDestination = t.transform;
    }

    // Update is called once per frame
    private void Update() {
        if (mDead) return;
        
        mAgent.destination = mDestination.position;
        healthText.text = health.ToString();
        
        healthTextTransform.LookAt(mDestination);
        healthTextTransform.forward = -healthTextTransform.forward; // Invert
    }

    public void DoDamage(int dmg) {
        health -= dmg;

        if (health <= 0) {
            mWave.Signal(this);
            mDead                  = true;
            mAgent.enabled         = false;
            mRigidbody.isKinematic = false;
            mRigidbody.AddForce(mDeathVec * 5f, ForceMode.Impulse);
            healthText.enabled = false;
            // Particle effect stuff
            return;
        }

        healthText.text = health.ToString();
    }
    
    public void SetWave(Wave wave) {
        mWave = wave;
    }

    public void SetDeathVector(Vector3 vec) {
        mDeathVec = vec;
    }
}