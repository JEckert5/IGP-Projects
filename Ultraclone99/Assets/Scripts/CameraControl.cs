using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class CameraControl : MonoBehaviour {

    public float sensitivity;
    private float mXRotation;
    public Camera playerCamera;
    private float mMaxXRot;

    private PlayerFPSControls mFPSControls;
    // Start is called before the first frame update
    private void Start() {
        mXRotation = 0f;
        mMaxXRot   = 89.9f;
    }

    private void Awake() {
        mFPSControls = new PlayerFPSControls();
    }

    // Update is called once per frame
    void Update() {
        var mouseVals = mFPSControls.Gameplay.look.ReadValue<Vector2>();
        var mouseX    = mouseVals.x * sensitivity * Time.deltaTime;
        var mouseY    = mouseVals.y * sensitivity * Time.deltaTime;

        mXRotation -= mouseY;
        mXRotation = Mathf.Clamp(mXRotation, -mMaxXRot, mMaxXRot);

        playerCamera.transform.localRotation = Quaternion.Euler(mXRotation, 0f, 0f); 
        transform.Rotate(Vector3.up * mouseX);
    }

    public void OnEnable() {
        mFPSControls.Enable();
    }

    public void OnDisable() {
        mFPSControls.Disable();
    }

}
