using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.UI;

public class SoldierMovement : MonoBehaviour {

    public float moveSpeed;

    private Movement mMovement;
    
    private Rigidbody2D mSoldier;

    private Animator mAnim;

    private Vector3 mPos;

    public Camera mainCamera;

    private int mSols;

    public Text sols;
    // Start is called before the first frame update
    void Start() {
        mSoldier = GetComponent<Rigidbody2D>();
        mAnim    = GetComponent<Animator>();
        moveSpeed *= 100;
        mSols = 0;
    }

    private void Awake() {
        mMovement = new Movement();
    }

    private void OnEnable() {
        mMovement.gameplay.Enable();
    }

    // Update is called once per frame
    void Update() {
        var move = mMovement.gameplay.move.ReadValue<Vector2>();
        var scroll = mMovement.gameplay.zoom.ReadValue<float>();

        mainCamera.orthographicSize -= scroll / 10 * Time.deltaTime;
        mainCamera.orthographicSize = Mathf.Clamp(mainCamera.orthographicSize, 3, 10);
        Debug.Log(mainCamera.orthographicSize);

        mPos = new Vector3(move.x * moveSpeed * Time.deltaTime, move.y * moveSpeed * Time.deltaTime, 0);

        mSoldier.linearVelocity = mPos;
        
        mAnim.SetFloat("Up", move.y);
        mAnim.SetFloat("Right", move.x);
    }

    private void OnTriggerEnter2D(Collider2D other) {
        if (other.CompareTag("SolCoin")) {
            other.gameObject.SetActive(false);
            mSols++;
            sols.text = "Sols: " + mSols.ToString();
        }
    }
}
