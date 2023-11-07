using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerController : MonoBehaviour {

    [SerializeField] private float movementSpeed;
    [SerializeField] private float sprintSpeed;
    [SerializeField] private float jumpHeight;
    [SerializeField] private float gravity;
    
    private PlayerFPSControls mInputs;
    
    private void Start() {
        
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
        
    }

    private void OnJump() {
        
    }

    private void OnSprintPress() {
        
    }

    private void OnSprintRelease() {
        
    }
}
