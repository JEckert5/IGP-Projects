using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerController : MonoBehaviour {

    [SerializeField] private float movementSpeed;
    [SerializeField] private float sprintSpeed;
    [SerializeField] private float jumpHeight;
    [SerializeField] private float gravity;
    [SerializeField] private Transform lookAt;
    [SerializeField] private Transform cameraPosition;
    
    private PlayerFPSControls mInputs;
    private CharacterController mCharacterController;
    private Vector3 mMove;
    
    private void Start() {
        mMove                = Vector3.zero;
        mCharacterController = GetComponent<CharacterController>();
    }

    private void Awake() {
        mInputs                           =  new PlayerFPSControls();
        mInputs.Gameplay.jump.performed   += _ => OnJump();
        mInputs.Gameplay.sprint.started   += _ => OnSprintPress();
        mInputs.Gameplay.sprint.performed += _ => OnSprintRelease();
    }

    private void OnEnable() {
        mInputs.Enable();
    }

    private void OnDisable() {
        mInputs.Disable();
    }
    
    private void Update() {
        var moveInput = mInputs.Gameplay.move.ReadValue<Vector2>();
        var lookInput = -mInputs.Gameplay.look.ReadValue<Vector2>();
        
        /*
         * Rotate around Player body when looking horizontal
         * Rotate `lookAt` around cameraPos X axis when vertical
         */

        transform.Rotate(Vector3.up, lookInput.x);

        var q = Quaternion.AngleAxis(lookInput.y, cameraPosition.right);
        lookAt.RotateAround(cameraPosition.position, cameraPosition.right, lookInput.y);

        mMove = transform.right * moveInput.x + transform.forward * moveInput.y;

        mCharacterController.Move(mMove * Time.deltaTime);
    }

    private void OnJump() {
        
    }

    private void OnSprintPress() {
        
    }

    private void OnSprintRelease() {
        
    }
}
