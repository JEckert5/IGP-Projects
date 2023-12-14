using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

/// <summary>
/// He tries to run you over.
/// </summary>
public class VolksBoss : MonoBehaviour {
    [SerializeField] private int health;
    
    private NavMeshAgent mAgent;
    private Transform mDestination;
    private bool mDmgDelay;
    
    private void Start() {
        mAgent = GetComponent<NavMeshAgent>();
        var p = GameObject.FindGameObjectWithTag("Player");

        if (p == null) {
            Debug.Log("Where is the player?????");
            throw new NullReferenceException("WHAT??????");
            return;
        }

        mDestination = p.transform;
    }

    private void Update() {
        mAgent.destination = mDestination.position;
    }

    private void OnCollisionEnter(Collision other) {
        if (!other.gameObject.CompareTag("Player") || mDmgDelay) return;

        var p = other.gameObject.GetComponent<PlayerController>();
        
        p.DoDamage(15);

        mDmgDelay = true;
        StartCoroutine(DmgDelay());
    }

    private IEnumerator DmgDelay() {
        yield return new WaitForSecondsRealtime(1f);

        mDmgDelay = false;
    }
}
