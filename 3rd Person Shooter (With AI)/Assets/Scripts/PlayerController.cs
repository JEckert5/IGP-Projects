using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.AI;
using UnityEngine.SceneManagement;

[RequireComponent(typeof(CharacterController), typeof(PlayerInput))]
public class PlayerController : MonoBehaviour
{
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

    private CharacterController controller;
    private PlayerInput playerInput;
    private Vector3 playerVelocity;
    private bool groundedPlayer;
    private Transform cameraTransform;

    private InputAction moveAction;
    private InputAction jumpAction;
    private InputAction shootAction;
    private InputAction menuAction;

    public NavMeshAgent cubert;

    private void Awake()
    {
        controller      = GetComponent<CharacterController>();
        playerInput     = GetComponent<PlayerInput>();
        cameraTransform = Camera.main.transform;
        moveAction      = playerInput.actions["Move"];
        jumpAction      = playerInput.actions["Jump"];
        shootAction     = playerInput.actions["Shoot"];
        menuAction      = playerInput.actions["Menu"];

        Cursor.visible = false;
        Cursor.lockState = CursorLockMode.Locked;
    }

    private void OnEnable()
    {
        shootAction.performed += _ => ShootGun();
        menuAction.performed  += _ => ToMenu();
    }
    private void OnDisable()
    {
        shootAction.performed -= _ => ShootGun();
        menuAction.performed  -= _ => ToMenu();
    }

    private void ShootGun()
    {
        RaycastHit hit;

        GameObject bullet = GameObject.Instantiate(bulletPrefab, shootingPart.position, Quaternion.identity, bulletParent);
        BulletController bulletController = bullet.GetComponent<BulletController>();
        
        if (Physics.Raycast(cameraTransform.position, cameraTransform.forward, out hit, Mathf.Infinity))
        {
            bulletController.target = hit.point;
            bulletController.hit = true;
            cubert.destination = hit.point;

        }
        else
        {
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

        controller.Move(move * Time.deltaTime * playerSpeed);



        // Changes the height position of the player..
        if (jumpAction.triggered && groundedPlayer)
        {
            playerVelocity.y += Mathf.Sqrt(jumpHeight * -3.0f * gravityValue);
        }

        playerVelocity.y += gravityValue * Time.deltaTime;
        controller.Move(playerVelocity * Time.deltaTime);

        // Rotate towards camera direction.
        if (input.x != 0 || input.y != 0)
        {
            float targetAngle = cameraTransform.eulerAngles.y;
            Quaternion targetRotation = Quaternion.Euler(0, targetAngle, 0);
            transform.rotation = Quaternion.Lerp(transform.rotation, targetRotation, rotationSpeed * Time.deltaTime);
        }
    }

    private void ToMenu() {
        SceneManager.LoadScene("MainMenu");
        Cursor.visible   = true;
        Cursor.lockState = CursorLockMode.None;
    }
}
