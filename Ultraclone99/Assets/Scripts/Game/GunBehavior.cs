using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;
using TMPro;

public class GunBehavior : MonoBehaviour {

    private PlayerFPSControls mFPSControls;

    public TextMeshProUGUI ammoText;

    private int mAmmoCount;
    private const int mMaxAmmo = 15;

    private bool reloading;
    public Transform gunTransform;
    
    // Hit scan
    private void Start() {
        mAmmoCount = mMaxAmmo;
        ammoText.text = mAmmoCount.ToString();
        reloading = false;
    }

    private void Awake() {
        mFPSControls                       =  new PlayerFPSControls();
        mFPSControls.Gameplay.fire.started += OnFire;
        mFPSControls.Gameplay.reload.started += OnReload;
    }
    
    private void Update() {
        
    }

    private void OnFire(InputAction.CallbackContext ctx) {
        if (mAmmoCount <= 0 || reloading) return;

        mAmmoCount -= 1;

        ammoText.text = mAmmoCount.ToString();

        if (Physics.Raycast(transform.position, transform.forward, out var hit)) {

            var prev = new Vector3(gunTransform.position.x, gunTransform.position.y, gunTransform.position.z);
            // Debug.Log(prev);
            gunTransform.LookAt(hit.point);
            
            Debug.DrawRay(gunTransform.position, gunTransform.forward * hit.distance, Color.green, 3);
            Debug.Log("hit");
            
            // Debug.Log(prev);

            gunTransform.position = prev;

            if (!hit.collider.CompareTag("Enemy")) return;

            var temp = hit.collider.gameObject.GetComponent<GunkyLadBehavior>();
            temp.health -= 10;
            
            
        } else {
            Debug.DrawRay(gunTransform.position, gunTransform.forward * 1000, Color.red, 3);
            Debug.Log("no hit");
        }
    }

    private void OnReload(InputAction.CallbackContext ctx) {
        // Debug.Log("Reloading...");
        ammoText.text = "Reloading...";
        mAmmoCount = mMaxAmmo;
        reloading = true;

        StartCoroutine(WaitForReload());
    }

    private IEnumerator WaitForReload() {
        yield return new WaitForSeconds(0.64f);
        
        Debug.Log("Done reloading");

        reloading = false;
        ammoText.text = mAmmoCount.ToString();
    }

    private void OnEnable() {
        mFPSControls.Enable();
    }

    private void OnDisable() {
        mFPSControls.Disable();
    }

}
