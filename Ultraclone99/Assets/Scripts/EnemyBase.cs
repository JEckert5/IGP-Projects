using System.Collections;
using TMPro;
using UnityEngine;
using UnityEngine.AI;

public abstract class EnemyBase : MonoBehaviour {
    protected NavMeshAgent agent;
    protected Transform destination;
    protected Wave wave;
    protected new Rigidbody rigidbody;
    protected bool dead;
    protected bool canShoot = true;
    protected Vector3 deathVec;
    protected Transform bulletParent;

    [SerializeField] protected Transform healthTextTransform;
    [SerializeField] protected int health;
    [SerializeField] protected TextMeshProUGUI healthText;
    [SerializeField] protected float shootTimer;
    [SerializeField] protected GameObject laserPrefab;
    [SerializeField] protected Transform shootPoint;
    [SerializeField] protected Transform hitRegPoint;
    [SerializeField] protected AudioSource gunSource;
    
    public abstract void DoDamage(int dmg);
    public abstract void SetWave(Wave newWave);
    public abstract void SetDeathVector(Vector3 vec);
    protected abstract void Shoot();
    protected abstract IEnumerator ShootTimer();
}
