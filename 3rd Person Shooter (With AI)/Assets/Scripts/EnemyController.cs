using System;
using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.AI;

public class EnemyController : MonoBehaviour {

    private NavMeshAgent agent;
    private Transform player;

    [SerializeField] private int health;

    [SerializeField] private GameObject bulletPrefab;
    [SerializeField] private TextMeshProUGUI healthText;

    [SerializeField] private Transform shootPoint;

    [SerializeField] private AudioSource gunSource;

    private Wave wave;
        
    private float shootTimer;
    private Transform bulletParent;
    const float ShootTimerDefault = 0.95f;

    private void Start() {
        agent           = GetComponent<NavMeshAgent>();
        player          = GameObject.FindGameObjectWithTag("Player").transform;
        shootTimer      = ShootTimerDefault;
        healthText.text = health.ToString();
        gunSource.spatialize = true;
    }

    private void Update() {
        agent.destination = player.position;
        healthText.transform.LookAt(player);

        // Flip text
        healthText.transform.forward = -healthText.transform.forward;

        shootTimer -= Time.deltaTime;

        if (shootTimer > 0) return; // Can shoot if <= 0

        shootTimer = ShootTimerDefault;

        Shoot();
    }

    private void Shoot() {
        var bullet     = Instantiate(bulletPrefab, transform.position, Quaternion.identity);
        var bulletController = bullet.GetComponent<BulletController>();
        shootPoint.GetComponentInChildren<ParticleSystem>().Play();
        bulletController.SetParent(gameObject);

        gunSource.Play();
        
        // Debug.Log("Shoot");
        
        if (Physics.Raycast(shootPoint.position, shootPoint.forward, out RaycastHit hit, Mathf.Infinity)) {
            bulletController.target = hit.point;
            bulletController.hit    = true;
        } else {
            bulletController.target = shootPoint.position + shootPoint.forward * 25;
            bulletController.hit    = false;
        }
    }

    public bool Damage(int dmg) {
        if (health <= 0) return false;
        
        health -= dmg;

        healthText.text = health.ToString();

        if (health > 0) return false;

        wave.Signal(this);
        AudioManager.instance.Play("WUHUH");
        // Destroy(gameObject);
        enabled = false;
        gameObject.GetComponentInChildren<ParticleSystem>().Play();
        return true;
    }

    public void SetWave(Wave w) {
        wave = w;
    }
}
