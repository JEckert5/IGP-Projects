using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class OpenDoor : MonoBehaviour {

    public Transform door;
    private PlayerFPSControls mSys;
    private bool mActive;

    void Awake() {
        mSys                             =  new PlayerFPSControls();
        mSys.Gameplay.interact.performed += InteractCallback;
    }

    private void OnTriggerEnter(Collider other) {
        if (!other.CompareTag("Player")) return;
        
        mActive = true;
    }

    private void OnTriggerExit(Collider other) {
        mActive = false;
    }

    private void InteractCallback(InputAction.CallbackContext ctx) {
        if (!mActive) return;

        var vel = 0f;
        
        Debug.Log("Hello?");

        var goal = door.position.y + 5;

        while (door.position.y < goal) {
            var dp   = door.position;
            var p    = Mathf.SmoothDamp(dp.y, goal, ref vel, 10f);
            door.position = new Vector3(dp.x, p, dp.z);
        }
    }

    private void OnEnable() {
        mSys.Enable();
    }
}
