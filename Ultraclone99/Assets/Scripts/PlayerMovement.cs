using System;
using System.Numerics;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.InputSystem;
using Vector3 = UnityEngine.Vector3;
using Vector2 = UnityEngine.Vector2;

public class PlayerMovement : MonoBehaviour {
    #region Movement
    
    public float movementSpeed;
    public float sprintSpeed;
    public float jumpHeight;
    private float mVelocity;
    private Vector3 mMove;
    private Vector2 mMomentum;
    private Vector2 mMoveVals;
    public float momentumStartTime;
    public float momentumStopTime;
    private Vector2 mMomentumVel;
    private bool[] mWindDown;
    public CapsuleCollider capsuleCollider;
    
    #endregion
    
    [Tooltip("Input as positive number.")] public float gravity;

    public CharacterController playerCharacterController;
    private PlayerFPSControls mFPSActions;
    
    private void Start() {
        mMove        = Vector3.zero;
        mVelocity    = 0f;
        mMomentum    = Vector2.zero;
        mMomentumVel = Vector2.zero;
        mWindDown    = new[] {true, true};
    }
    
    public void Awake() {
        mFPSActions                           =  new PlayerFPSControls();
        mFPSActions.Gameplay.jump.performed   += OnJump;
        mFPSActions.Gameplay.sprint.started   += OnSprintPress;
        mFPSActions.Gameplay.sprint.performed += OnSprintRelease;
        mFPSActions.Gameplay.crouch.started   += OnCrouchPress;
        mFPSActions.Gameplay.crouch.performed += OnCrouchRelease;
    }

    // Update is called once per frame
    private void Update() {
        var tempMove = mFPSActions.Gameplay.move.ReadValue<Vector2>();

        if (tempMove.x != 0f) {
            mMoveVals.x  = tempMove.x;
            mWindDown[0] = false;
        } else {
            mWindDown[0] = true;
        }

        if (tempMove.y != 0f) {
            mMoveVals.y  = tempMove.y;
            mWindDown[1] = false;
        } else {
            mWindDown[1] = true;
        }
        
        UpdateMomentum();
        
        mMove = transform.right * (mMoveVals.x * mMomentum.x) + transform.forward * (mMoveVals.y * mMomentum.y);
        
        UpdateGravity();
        
        playerCharacterController.Move(mMove * (movementSpeed * Time.deltaTime));
    }

    private void UpdateGravity() {
        if (playerCharacterController.isGrounded && mVelocity < 0f) {
            mVelocity = -1f;
        } else {
            mVelocity += -gravity * Time.deltaTime;
        }

        mMove.y = mVelocity;
    }

    private void UpdateMomentum() {
        if (mWindDown[0] && mMomentum.x != 0f) {
            mMomentum.x = Mathf.SmoothDamp(mMomentum.x, 0f, ref mMomentumVel.x, momentumStopTime);
        } else if (!mWindDown[0] && mMomentum.x <= 1f) {
            mMomentum.x = Mathf.SmoothDamp(mMomentum.x, 1f, ref mMomentumVel.x, momentumStartTime);
        }
        
        if (mWindDown[1] && mMomentum.y != 0f) {
            mMomentum.y = Mathf.SmoothDamp(mMomentum.y, 0f, ref mMomentumVel.y, momentumStopTime);
        } else if (!mWindDown[1] && mMomentum.y <= 1f) {
            mMomentum.y = Mathf.SmoothDamp(mMomentum.y, 1f, ref mMomentumVel.y, momentumStartTime);
        }
        
    }

    private void OnJump(InputAction.CallbackContext ctx) {
        if (playerCharacterController.isGrounded && mVelocity < 0f)
            mVelocity += jumpHeight;
    }

    private void OnSprintPress(InputAction.CallbackContext ctx) {
        movementSpeed *= sprintSpeed;
    }

    private void OnSprintRelease(InputAction.CallbackContext ctx) {
        movementSpeed /= sprintSpeed;
    }

    private void OnCrouchPress(InputAction.CallbackContext ctx) {
        
    }

    private void OnCrouchRelease(InputAction.CallbackContext ctx) {
        
    }

    private void OnControllerColliderHit(ControllerColliderHit hit) {
        if (!playerCharacterController.isGrounded && hit.collider.CompareTag("Ceiling"))
            mVelocity = -mVelocity * Time.deltaTime;
    }

    public void OnEnable() {
       mFPSActions.Gameplay.Enable();
    }

    public void OnDisable() {
        mFPSActions.Gameplay.Disable();
    }
}
