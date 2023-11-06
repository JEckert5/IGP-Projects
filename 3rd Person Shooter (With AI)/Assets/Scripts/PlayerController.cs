using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.AI;
using UnityEngine.SceneManagement;
using TMPro;
using UnityEditor;

[RequireComponent(typeof(CharacterController), typeof(PlayerInput))]
public class PlayerController : MonoBehaviour {
    [SerializeField]
    private float playerSpeed = 2.0f;
    [SerializeField]
    private float jumpHeight = 1.0f;
    [SerializeField]
    private float gravityValue = -9.81f;
    [SerializeField]
    private float rotationSpeed = 0.8f;
    [SerializeField]
    private GameObject bulletPrefab;
    [SerializeField]
    private Transform shootingPart;
    [SerializeField]
    private Transform bulletParent;
    [SerializeField]
    private float bulletHitMissDistance = 25f;
    [SerializeField] 
    private int maxAmmo = 15;
    [SerializeField] private AudioSource amogus;

    private int health = 100;
    
    private CharacterController controller;
    private PlayerInput playerInput;
    private Vector3 playerVelocity;
    private bool groundedPlayer;
    private Transform cameraTransform;
    private int currentAmmo;
    public int currentReserveAmmo; // Public so AmmoPickup can edit
    private bool reloading;

    private InputAction moveAction;
    private InputAction jumpAction;
    private InputAction shootAction;
    private InputAction menuAction;
    private InputAction reloadAction;

    public TextMeshProUGUI ammoText;
    public TextMeshProUGUI reserveText;
    public TextMeshProUGUI healthText;

    public Canvas ammoCanvas;
    public Canvas gameOverCanvas;

    private int currentScore;

    private void Awake() {
        currentAmmo = maxAmmo;
        ammoText.text = currentAmmo.ToString();
        currentReserveAmmo = maxAmmo * 3;
        reserveText.text = currentReserveAmmo.ToString();
        controller      = GetComponent<CharacterController>();
        playerInput     = GetComponent<PlayerInput>();
        cameraTransform = Camera.main!.transform;
        moveAction      = playerInput.actions["Move"];
        jumpAction      = playerInput.actions["Jump"];
        shootAction     = playerInput.actions["Shoot"];
        menuAction      = playerInput.actions["Menu"];
        reloadAction = playerInput.actions["Reload"];

        gameOverCanvas.enabled = false;

        healthText.text = health.ToString();
        
        Cursor.visible = false;
        Cursor.lockState = CursorLockMode.Locked;

        amogus.spatialize = true;
    }

    private void OnEnable()
    {
        shootAction.performed += _ => ShootGun();
        menuAction.performed  += _ => ToMenu();
        reloadAction.performed += _ => Reload();
    }
    
    private void OnDisable() {
        shootAction.performed -= _ => ShootGun();
        menuAction.performed  -= _ => ToMenu();
        reloadAction.performed -= _ => Reload();
    }

    private void ShootGun()
    {
        if (currentAmmo <= 0 || reloading) return;

        amogus.Play();
        
        GameObject bullet = GameObject.Instantiate(bulletPrefab, shootingPart.position, Quaternion.identity, bulletParent);
        BulletController bulletController = bullet.GetComponent<BulletController>();
        
        bulletController.SetParent(gameObject);
        ParticleSystem ps = shootingPart.GetComponentInChildren<ParticleSystem>();

        ps.Play();

        currentAmmo -= 1;
        ammoText.text = currentAmmo.ToString();
        
        if (Physics.Raycast(cameraTransform.position, cameraTransform.forward, out RaycastHit hit, Mathf.Infinity)) {
            bulletController.target = hit.point;
            bulletController.hit = true;
        } else {
            bulletController.target = cameraTransform.position + cameraTransform.forward * 25;
            bulletController.hit = false;
        }
    }

    void Update()
    {
        groundedPlayer = controller.isGrounded;
        if (groundedPlayer && playerVelocity.y < 0)
        {
            playerVelocity.y = 0f;
        }

        Vector2 input = moveAction.ReadValue<Vector2>();
        Vector3 move = new Vector3(input.x, 0, input.y);

        move = move.x * cameraTransform.right.normalized +
               move.z * cameraTransform.forward.normalized;
        move.y = 0f;

        controller.Move(move * (Time.deltaTime * playerSpeed));
        
        // Changes the height position of the player..
        if (jumpAction.triggered && groundedPlayer) {
            playerVelocity.y += Mathf.Sqrt(jumpHeight * -3.0f * gravityValue);
        }

        playerVelocity.y += gravityValue * Time.deltaTime;
        controller.Move(playerVelocity * Time.deltaTime);

        // Rotate towards camera direction.
        if (input.x != 0 || input.y != 0) {
            float targetAngle = cameraTransform.eulerAngles.y;
            Quaternion targetRotation = Quaternion.Euler(0, targetAngle, 0);
            transform.rotation = Quaternion.Lerp(transform.rotation, targetRotation, rotationSpeed * Time.deltaTime);
        }
    }

    public void ToMenu() {
        SceneManager.LoadScene("MainMenu");
        Cursor.visible   = true;
        Cursor.lockState = CursorLockMode.None;
        AudioManager.instance.StopAllSounds();
        
        UpdateHighScore();
    }

    private void Reload() {
        if (currentReserveAmmo <= 0) return;
        
        ammoText.text = "Reloading...";
        reloading = true;
        int prevRA = currentReserveAmmo;
        currentReserveAmmo -= maxAmmo - currentAmmo;

        currentAmmo = prevRA <= 15 ? prevRA : maxAmmo % prevRA;

        // Clamp
        if (currentReserveAmmo < 0) currentReserveAmmo = 0;
        
        StartCoroutine(ReloadTimer());
    }

    private IEnumerator ReloadTimer() {
        yield return new WaitForSeconds(0.6f);

        reloading = false;
        
        reserveText.text = currentReserveAmmo.ToString();
        ammoText.text = currentAmmo.ToString();
    }

    public void Damage(int dmg) {
        health -= dmg;

        if (health <= 0) {
            GameOver();
        }

        healthText.text = health.ToString();
    }

    private void GameOver() {
        Time.timeScale = 0;
        
        Cursor.visible   = true;
        Cursor.lockState = CursorLockMode.None;

        ammoCanvas.enabled     = false;
        gameOverCanvas.enabled = true;
        
        UpdateHighScore();
    }

    public void AddScore() {
        currentScore += 1;
    }

    private void UpdateHighScore() {
        var hs = PlayerPrefs.GetInt("score", 0);

        if (hs < currentScore) {
            PlayerPrefs.SetInt("score", currentScore);
        }
    }
}
