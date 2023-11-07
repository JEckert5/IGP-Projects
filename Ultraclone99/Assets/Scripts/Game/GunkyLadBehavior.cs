using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;
using TMPro;
using UnityEngine.SceneManagement;

public class GunkyLadBehavior: MonoBehaviour {

    private NavMeshAgent mAgent;
    private Transform mDestination;
    private Wave mWave;

    [SerializeField] private int health;

    public TextMeshProUGUI healthText;

    // Start is called before the first frame update
    private void Start() {
        mAgent = GetComponent<NavMeshAgent>();

        var t = GameObject.FindGameObjectWithTag("Player");

        if (t == null) return;

        mDestination = t.transform;
    }

    // Update is called once per frame
    private void Update() {
        if (health <= 0) {
            mWave.Signal(this);

            return;
        }
        
        mAgent.destination = mDestination.position;
        healthText.text = health.ToString();
    }

    public void DoDamage(int dmg) {
        health -= dmg;
    }
    
    public void SetWave(Wave wave) {
        mWave = wave;
    }
}