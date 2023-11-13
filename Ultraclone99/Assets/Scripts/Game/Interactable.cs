using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Interactable : MonoBehaviour {
    // Active is only true if Player is in trigger
    protected bool active;

    protected void OnTriggerEnter(Collider other) {
        active = other.CompareTag("Player");

        if (!active) return;
        
        Debug.Log("Enter Collider");

        var p = other.GetComponent<PlayerController>();

        p.interactable = this;
    }

    protected void OnTriggerExit(Collider other) {
        if (active) {
            var p = other.GetComponent<PlayerController>();
            p.interactable = null;
        }

        active = false;
    }

    public virtual void Action() { }

}
