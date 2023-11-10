using System;
using System.Collections;
using System.Collections.Generic;
using Unity.Mathematics;
using UnityEngine;
using UnityEngine.Networking;

public class PlayerController : MonoBehaviour {

    [SerializeField] private float movementSpeed;
    [SerializeField] private float sprintSpeed;
    [SerializeField] private float jumpHeight;
    [SerializeField] private Transform cameraPosition;
    [SerializeField] private GameObject laserPrefab;
    [SerializeField] private Transform shootPoint;
    
    private PlayerFPSControls mInputs;
    private CharacterController mCharacterController;
    private Vector3 mMove;

    private float mXRotation;
    private const float MaxRotation = 89.9f;
    private const float Gravity = -9.81f;
    
    private void Start() {
        mMove                = Vector3.zero;
        mCharacterController = GetComponent<CharacterController>();
        mXRotation           = 0f;

        Cursor.visible   = false;
        Cursor.lockState = CursorLockMode.Locked;
    }

    private void Awake() {
        mInputs                           =  new PlayerFPSControls();
        mInputs.Gameplay.jump.performed   += _ => OnJump();
        mInputs.Gameplay.sprint.started   += _ => OnSprintPress();
        mInputs.Gameplay.sprint.performed += _ => OnSprintRelease();
        mInputs.Gameplay.fire.started     += _ => OnShoot();
    }

    private void OnEnable() {
        mInputs.Enable();
    }

    private void OnDisable() {
        mInputs.Disable();
    }
    
    private void Update() {
        var moveInput = mInputs.Gameplay.move.ReadValue<Vector2>();
        var lookInput = mInputs.Gameplay.look.ReadValue<Vector2>();
        
        /*
         * Rotate around Player body when looking horizontal.
         * Rotate cameraPosition X for vertical.
         */

        if (lookInput == Vector2.zero) goto doMove;
        
        transform.Rotate(Vector3.up, lookInput.x);
        mXRotation                   -= lookInput.y;
        mXRotation                   =  Mathf.Clamp(mXRotation, -MaxRotation, MaxRotation);
        cameraPosition.localRotation =  Quaternion.Euler(mXRotation, 0, 0);

    doMove:
        if (moveInput == Vector2.zero) return;
        
        mMove = transform.right * moveInput.x + transform.forward * moveInput.y;

        mMove.y += Gravity * Time.deltaTime;
        
        mCharacterController.Move(mMove * (movementSpeed * Time.deltaTime));
    }

    private void OnJump() {
        if (!mCharacterController.isGrounded) return;

        mMove.y = Mathf.Sqrt(jumpHeight * -2f * Gravity);
    }

    private void OnSprintPress() {
        movementSpeed *= sprintSpeed;
    }

    private void OnSprintRelease() {
        movementSpeed /= sprintSpeed;
    }

    private void OnShoot() {
        // Use camera for hit reg, then shoot from shootpoint.
        if (Physics.Raycast(cameraPosition.position, cameraPosition.forward, out var hit)) {
            var laser = Instantiate(laserPrefab, shootPoint.position, Quaternion.identity);
            var lc    = laser.GetComponent<Laser>();
            
            lc.SetTarget(hit.transform);

            Debug.DrawRay(cameraPosition.position, cameraPosition.forward * hit.distance, Color.blue);
        }
    }
}
