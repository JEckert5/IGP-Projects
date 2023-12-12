using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;
using TMPro;
using UnityEngine.SceneManagement;
using UnityEngine.Serialization;
using Random = System.Random;

public class GunkyLadController: MonoBehaviour {

    private NavMeshAgent mAgent;
    private Transform mDestination; // Player
    private Wave mWave;
    private Rigidbody mRigidbody;
    private bool mDead;
    private bool mCanShoot = true;
    private Vector3 mDeathVec; // Direction he flies when dead 
    private Transform mBulletParent;
    
    [SerializeField] private Transform healthTextTransform;
    [SerializeField] private int health;
    [SerializeField] private TextMeshProUGUI healthText;
    [SerializeField] private float shootTimer;
    [SerializeField] private GameObject laserPrefab;
    [SerializeField] private Transform shootPoint;
    
    // Start is called before the first frame update
    private void Start() {
        mAgent                      = GetComponent<NavMeshAgent>();
        mRigidbody                  = GetComponent<Rigidbody>();
        mRigidbody.detectCollisions = true;
        var t = GameObject.FindGameObjectWithTag("Player");

        if (t == null) return;

        mDestination = t.transform;
        mBulletParent = GameObject.FindGameObjectWithTag("BulletParent").transform;
    }

    // Update is called once per frame
    private void Update() {
        if (mDead) return;
        
        mAgent.destination = mDestination.position;
        healthText.text = health.ToString();
        
        healthTextTransform.LookAt(mDestination);
        healthTextTransform.forward = -healthTextTransform.forward; // Invert

        if (mCanShoot) Shoot();
    }

    public void DoDamage(int dmg) {
        health -= dmg;

        if (health <= 0) {
            mWave.Signal(this);
            mDead                  = true;
            mAgent.enabled         = false;
            mRigidbody.isKinematic = false;
            mRigidbody.useGravity  = true;
            mRigidbody.AddForce(mDeathVec * 5f, ForceMode.Impulse);
            healthText.enabled = false;
            // Particle effect stuff
            return;
        }

        healthText.text = health.ToString();
    }
    
    public void SetWave(Wave wave) {
        mWave = wave;
    }

    public void SetDeathVector(Vector3 vec) {
        mDeathVec = vec;
    }

    private void Shoot() {
        mCanShoot = false;
        StartCoroutine(ShootTimer());

        var position = shootPoint.position;
        var laser = Instantiate(laserPrefab, position, Quaternion.identity);
        var lc = laser.GetComponent<Laser>();

        if (Physics.Raycast(shootPoint.position, shootPoint.forward, out var hit) &&
            hit.collider.gameObject.CompareTag("Player")) {
            lc.SetTarget(hit.point, position);

            var player = hit.collider.gameObject.GetComponent<PlayerController>();

            player.DoDamage(5);
        }
        else {
            lc.SetTarget(position + shootPoint.forward * 200f, position);
        }
    }

    private IEnumerator ShootTimer() {
        var random = new Random();
        yield return new WaitForSeconds(shootTimer + (float)random.NextDouble());

        mCanShoot = true;
    }
}