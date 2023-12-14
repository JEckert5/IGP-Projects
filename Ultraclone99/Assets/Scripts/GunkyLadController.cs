using System.Collections;
using UnityEngine;
using UnityEngine.AI;
using TMPro;
using Random = System.Random;

public class GunkyLadController: EnemyBase {

    [SerializeField] private ParticleSystem sustem;
    
    private void Start() {
        agent                      = GetComponent<NavMeshAgent>();
        rigidbody                  = GetComponent<Rigidbody>();
        rigidbody.detectCollisions = true;
        var t = GameObject.FindGameObjectWithTag("Player");

        if (t == null) return;

        destination = t.transform;
        bulletParent = GameObject.FindGameObjectWithTag("BulletParent").transform;
    }
    
    private void Update() {
        if (dead) return;
        
        agent.destination = destination.position;
        healthText.text    = health.ToString();
        
        healthTextTransform.LookAt(destination);
        healthTextTransform.forward = -healthTextTransform.forward; // Invert

        var  rayDir = destination.position - transform.position;
        var los    = Physics.Raycast(transform.position, rayDir, out var hit) && hit.transform == destination;

        if (canShoot && los) Shoot();
    }

    public override void DoDamage(int dmg) {
        if (dead) return;
        
        health -= dmg;

        if (health <= 0) {
            wave.Signal(this);
            dead                  = true;
            agent.enabled         = false;
            rigidbody.isKinematic = false;
            rigidbody.useGravity  = true;
            rigidbody.AddForce(deathVec * 5f, ForceMode.Impulse);
            healthText.enabled = false;
            // Particle effect stuff
            sustem.Play();
            return;
        }

        healthText.text = health.ToString();
    }
    
    public override void SetWave(Wave newWave) {
        this.wave = newWave;
    }

    public override void SetDeathVector(Vector3 vec) {
        deathVec = vec;
    }

    protected override void Shoot() {
        canShoot = false;
        gunSource.Play();
        StartCoroutine(ShootTimer());

        var position = shootPoint.position;
        var laser = Instantiate(laserPrefab, position, Quaternion.identity);
        var lc = laser.GetComponent<Laser>();

        if (Physics.Raycast(hitRegPoint.position, hitRegPoint.forward, out var hit) &&
            hit.collider.gameObject.CompareTag("Player")) {
            lc.SetTarget(hit.point, position);

            var player = hit.collider.gameObject.GetComponent<PlayerController>();

            player.DoDamage(5);
        }
        else {
            lc.SetTarget(position + shootPoint.forward * 200f, position);
        }
    }

    protected override IEnumerator ShootTimer() {
        var random = new Random();
        yield return new WaitForSeconds(shootTimer + (float)random.NextDouble());

        canShoot = true;
    }
}