using System;
using System.Collections;
using UnityEngine;
using UnityEngine.AI;

/// <summary>
/// He tries to run you over.
/// </summary>
public class VolksBoss : EnemyBase {
    
    private void Start() {
        agent     = GetComponent<NavMeshAgent>();
        rigidbody = GetComponent<Rigidbody>();
        var p = GameObject.FindGameObjectWithTag("Player");

        if (p == null) {
            Debug.Log("Where is the player?????");
            throw new NullReferenceException("WHAT??????");
        }

        destination = p.transform;
    }

    private void Update() {
        if (dead) return;
        
        agent.destination = destination.position;
    }

    private void OnCollisionEnter(Collision other) {
        if (!other.gameObject.CompareTag("Player") || !canShoot) return;

        var p = other.gameObject.GetComponent<PlayerController>();
        
        p.DoDamage(15);

        canShoot = false;
        StartCoroutine(ShootTimer());
    }

    private void OnCollisionStay(Collision other) {
        if (!other.gameObject.CompareTag("Player") || !canShoot) return;

        var p = other.gameObject.GetComponent<PlayerController>();
        
        p.DoDamage(15);

        canShoot = true;
        StartCoroutine(ShootTimer());
    }

    public override void DoDamage(int dmg) {
        if (dead) return;
        
        health -= dmg;

        if (health <= 0) {
            // Death
            wave.Signal(this);
            dead = true;
            rigidbody.AddForce(-transform.forward * 6, ForceMode.Impulse);
            base.agent.enabled = false;
            gunSource.Stop();
        }
    }

    public override void SetWave(Wave newWave) {
        wave = newWave;
    }

    public override void SetDeathVector(Vector3 vec) {
        // Do nothing
    }

    protected override void Shoot() {
        // No Shoot
    }

    public void Stop() {
        gunSource.Stop();
    }

    protected override IEnumerator ShootTimer() {
        yield return new WaitForSecondsRealtime(1f);

        canShoot = true;
    }
}
