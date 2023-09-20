using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class GunBehavior : MonoBehaviour {

    private PlayerFPSControls mFPSControls;
    private RaycastHit hit;
    // Hit scan
    // Start is called before the first frame update
    private void Start() {
        
    }

    private void Awake() {
        mFPSControls                       =  new PlayerFPSControls();
        mFPSControls.Gameplay.fire.started += OnFire;
    }

    // Update is called once per frame
    private void Update() {
        // Debug.DrawRay(transform.position, transform.TransformDirection(transform.forward) * 1000, Color.red, 3);
    }

    private void OnFire(InputAction.CallbackContext ctx) {
        // RaycastHit hit;
        
        Debug.Log(transform.forward);
        Debug.Log(transform.TransformDirection(transform.forward));

        if (Physics.Raycast(transform.position, transform.forward, out hit)) {
            Debug.DrawRay(transform.position, transform.forward * hit.distance, Color.green, 3);
            Debug.Log("hit");
        } else {
            Debug.DrawRay(transform.position, transform.forward * 1000, Color.red, 3);
            Debug.Log("no hit");
        }
        
        // Debug.DrawRay(transform.position, transform.TransformDirection(transform.forward) * 1000, Color.blue, 3);
    }

    private void OnEnable() {
        mFPSControls.Enable();
    }

    private void OnDisable() {
        mFPSControls.Disable();
    }

}
