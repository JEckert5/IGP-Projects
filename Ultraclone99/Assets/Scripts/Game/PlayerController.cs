using System;
using System.Collections;
using TMPro;
using UnityEngine;

public class PlayerController : MonoBehaviour {

    #region Cereal

    [SerializeField] private float health;
    [SerializeField] private Transform cameraPosition;
    [SerializeField] private float sensitivity;
    [SerializeField] private float gravity;

    #endregion

    #region Gun

    [SerializeField] private TextMeshProUGUI ammoText;
    [SerializeField] private TextMeshProUGUI healthText;
    [SerializeField] private TextMeshProUGUI reserveText;
    [SerializeField] private float reloadTime;
    [SerializeField] private int maxMagazine;
    [SerializeField] private int maxReserve;
    [SerializeField] private GameObject laserPrefab;
    [SerializeField] private Transform shootPoint;
    private int mCurrentAmmo;
    private int mReserveAmmo;
    private bool mReloading;
    private bool mIsDelaying;

    #endregion

    #region Movement

    [SerializeField] private float movementSpeed;
    [SerializeField] private float sprintSpeed;
    [SerializeField] private float jumpHeight;
    [SerializeField] private float accelerationTime;
    [SerializeField] private float decelerationTime;
    private PlayerFPSControls mInputs;
    private CharacterController mCharacterController;
    private Vector3 mMove;
    private float mYVeloctiy;
    private Vector2 mMomentum;
    private Vector2 mRefVel;
    private float mXRotation;
    private const float MaxRotation = 89.9f;

    #endregion

    private Interactable mInteractable;

    private void Start() {
        mMove                = Vector3.zero;
        mCharacterController = GetComponent<CharacterController>();
        mXRotation           = 0f;
        mMomentum            = Vector2.zero;
        mInteractable        = null;
        mCurrentAmmo         = maxMagazine;
        mReserveAmmo         = maxReserve / 3;
        healthText.text      = "Health: " + health;
        ammoText.text        = mCurrentAmmo.ToString();
        reserveText.text     = mReserveAmmo.ToString();
        Debug.Log(mInputs.Gameplay.fire.interactions);

        Cursor.visible   = false;
        Cursor.lockState = CursorLockMode.Locked;
    }

    private void Awake() {
        mInputs                             =  new PlayerFPSControls();
        mInputs.Gameplay.jump.performed     += _ => OnJump();
        mInputs.Gameplay.sprint.started     += _ => OnSprintPress();
        mInputs.Gameplay.sprint.performed   += _ => OnSprintRelease();
        mInputs.Gameplay.interact.performed += _ => Interact();
        mInputs.Gameplay.reload.started     += _ => Reload();
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

        // Update momentum before applying move updates.
        Momentum(moveInput);

        /*
         * Rotate around Player body when looking horizontal.
         * Rotate cameraPosition X for vertical.
         */

        if (lookInput == Vector2.zero) goto doMove;

        transform.Rotate(Vector3.up, lookInput.x * Time.deltaTime * sensitivity);
        mXRotation                   -= lookInput.y * Time.deltaTime * sensitivity;
        mXRotation                   =  Mathf.Clamp(mXRotation, -MaxRotation, MaxRotation);
        cameraPosition.localRotation =  Quaternion.Euler(mXRotation, 0, 0);

    doMove:
        mMove = transform.right * mMomentum.x + transform.forward * mMomentum.y;

        // Gravity
        if (mCharacterController.isGrounded && mYVeloctiy < 0f)
            mYVeloctiy = -1f;
        else
            mYVeloctiy += gravity * Time.deltaTime;

        mMove.y = mYVeloctiy;

        mCharacterController.Move(mMove * (movementSpeed * Time.deltaTime));
        
        if (mInputs.Gameplay.fire.IsPressed()) Shoot();
    }

    private void Momentum(Vector2 inputs) {
        if (inputs != Vector2.zero) {
            mMomentum.x = Mathf.SmoothDamp(mMomentum.x, inputs.x, ref mRefVel.x, accelerationTime);
            mMomentum.y = Mathf.SmoothDamp(mMomentum.y, inputs.y, ref mRefVel.y, accelerationTime);
        } else {
            mMomentum.x = Mathf.SmoothDamp(mMomentum.x, inputs.x, ref mRefVel.x, decelerationTime);
            mMomentum.y = Mathf.SmoothDamp(mMomentum.y, inputs.y, ref mRefVel.y, decelerationTime);
        }
    }

    private void OnJump() {
        if (!mCharacterController.isGrounded) return;

        // Why yes I did take this from Brackeys 🐐
        mYVeloctiy = Mathf.Sqrt(jumpHeight * -2 * gravity);
    }

    private void OnSprintPress() {
        movementSpeed *= sprintSpeed;
    }

    private void OnSprintRelease() {
        movementSpeed /= sprintSpeed;
    }

    private void Shoot() {
        if (mReloading || mIsDelaying || mCurrentAmmo <= 0) return;

        mCurrentAmmo  -= 1;
        ammoText.text =  mCurrentAmmo.ToString();
        mIsDelaying   =  true;
        StartCoroutine(ShootDelay());

        // Use camera for hit reg, then shoot from shootpoint.
        var position = shootPoint.position;
        var laser    = Instantiate(laserPrefab, position, Quaternion.identity, shootPoint);
        var lc       = laser.GetComponent<Laser>();

        if (Physics.Raycast(cameraPosition.position, cameraPosition.forward, out var hit) && hit.distance <= 200f)
            lc.SetTarget(hit.point, position);
        else {
            lc.SetTarget(shootPoint.forward * 200f + position, position);

            return;
        }

        if (!hit.collider.CompareTag("Enemy")) return;

        var enemy = hit.collider.gameObject.GetComponent<GunkyLadController>();
        enemy.DoDamage(15);
    }

    private void Interact() {
        // Debug.Log("interact: " + mInteractable);

        if (mInteractable == null) return;

        mInteractable.Action();
    }

    private void OnControllerColliderHit(ControllerColliderHit hit) {
        if (!mCharacterController.isGrounded && hit.collider.CompareTag("Ceiling"))
            mYVeloctiy = -mYVeloctiy * Time.deltaTime;
    }

    public void SetInteractable(Interactable i) {
        mInteractable = i;
    }

    public void DoDamage(float dmg) {
        health -= dmg;

        healthText.text = "Health: " + health;
    }

    private void Reload() {
        if (mReserveAmmo <= 0 || mReloading) return; // Play click sound when out of ammo.

        mReloading    = true;
        ammoText.text = "...";
        var prevResAmmo = mReserveAmmo;
        mReserveAmmo -= maxMagazine - mCurrentAmmo;

        mCurrentAmmo = prevResAmmo <= maxMagazine ? prevResAmmo : maxMagazine % prevResAmmo;

        mReserveAmmo = Mathf.Clamp(mReserveAmmo, 0, maxReserve);

        StartCoroutine(ReloadTimer());
    }

    private IEnumerator ReloadTimer() {
        yield return new WaitForSeconds(reloadTime);

        mReloading       = false;
        ammoText.text    = mCurrentAmmo.ToString();
        reserveText.text = mReserveAmmo.ToString();
    }

    private IEnumerator ShootDelay() {
        yield return new WaitForSeconds(0.099f);

        mIsDelaying = false;
    }

}