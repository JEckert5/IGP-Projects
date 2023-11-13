using System;
using System.Collections;
using System.Collections.Generic;
using Unity.Mathematics;
using UnityEngine;
using UnityEngine.Networking;

public class PlayerController : MonoBehaviour {

    public Interactable interactable;

    [SerializeField] private float movementSpeed;
    [SerializeField] private float sprintSpeed;
    [SerializeField] private float jumpHeight;
    [SerializeField] private Transform cameraPosition;
    [SerializeField] private GameObject laserPrefab;
    [SerializeField] private Transform shootPoint;
    
    private PlayerFPSControls mInputs;
    private CharacterController mCharacterController;
    private Vector3 mMove;
    private float mYVeloctiy;

    private float mXRotation;
    private const float MaxRotation = 89.9f;
    private const float Gravity = -9.81f;
    
    private void Start() {
        mMove                = Vector3.zero;
        mCharacterController = GetComponent<CharacterController>();
        mXRotation           = 0f;
        interactable         = null;

        Cursor.visible   = false;
        Cursor.lockState = CursorLockMode.Locked;
    }

    private void Awake() {
        mInputs                           =  new PlayerFPSControls();
        mInputs.Gameplay.jump.performed   += _ => OnJump();
        mInputs.Gameplay.sprint.started   += _ => OnSprintPress();
        mInputs.Gameplay.sprint.performed += _ => OnSprintRelease();
        mInputs.Gameplay.fire.started     += _ => OnShoot();
        mInputs.Gameplay.interact.performed += _ => Interact();
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
        mMove = transform.right * moveInput.x + transform.forward * moveInput.y;
        
        // Gravity
        if (mCharacterController.isGrounded && mYVeloctiy < 0f)
            mYVeloctiy = -1f;
        else
            mYVeloctiy += Gravity * Time.deltaTime;

        mMove.y = mYVeloctiy;
        
        // Momentum
        
        mCharacterController.Move(mMove * (movementSpeed * Time.deltaTime));
    }

    private void OnJump() {
        if (!mCharacterController.isGrounded) return;

        // Debug.Log("JUMP");
        // Why yes I did take this from Brackeys 🐐
        mYVeloctiy = Mathf.Sqrt(jumpHeight * -2 * Gravity);
    }

    private void OnSprintPress() {
        movementSpeed *= sprintSpeed;
    }

    private void OnSprintRelease() {
        movementSpeed /= sprintSpeed;
    }

    private void OnShoot() {
        // Use camera for hit reg, then shoot from shootpoint.
        var position = shootPoint.position;
        var laser = Instantiate(laserPrefab, position, Quaternion.identity, shootPoint);
        var lc = laser.GetComponent<Laser>();
        
        if (Physics.Raycast(cameraPosition.position, cameraPosition.forward, out var hit))
            lc.SetTarget(hit.point, position);
        else {
            lc.SetTarget(shootPoint.forward * 1000f + position, position);

            return;
        }

        if (!hit.collider.CompareTag("Enemy")) return;

        var enemy = hit.collider.gameObject.GetComponent<GunkyLadController>();
        enemy.DoDamage(15);
    }

    private void Interact() {
        Debug.Log("interact: " + interactable);

        if (interactable == null) return;
        
        interactable.Action();
    }
}
