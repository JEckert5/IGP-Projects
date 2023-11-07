using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;
using TMPro;
using UnityEngine.SceneManagement;

public class GunkyLadBehavior: MonoBehaviour {

    private NavMeshAgent mAgent;
    public Transform destination;

    public int health;

    public TextMeshProUGUI healthText;

    private WaveManager mWaveManager;

    // Start is called before the first frame update
    void Start() {
        mAgent = GetComponent<NavMeshAgent>();
        // mAgent.destination = destination.position;

        var t = GameObject.FindGameObjectWithTag("Player");

        if (t == null) return;

        destination = t.transform;
    }

    // Update is called once per frame
    void Update() {
        mAgent.destination = destination.position;

        if (health <= 0) {
            // mWaveManager.KillMe(this);
        }
        
        healthText.text = health.ToString();
    }

    public void DoDamage(int dmg) {
        this.health -= dmg;
        // Debug.Log(health);
    }
    
    public void SetSpawnManager(WaveManager waveMan) {
        mWaveManager = waveMan;
    }
}