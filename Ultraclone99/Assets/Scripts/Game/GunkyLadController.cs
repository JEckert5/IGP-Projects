using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;
using TMPro;
using UnityEngine.SceneManagement;
using UnityEngine.Serialization;

public class GunkyLadController: MonoBehaviour {

    private NavMeshAgent mAgent;
    private Transform mDestination; // Player
    private Wave mWave;
    
    [SerializeField] private Transform healthTextTransform;
    [SerializeField] private int health;
    [SerializeField] private TextMeshProUGUI healthText;

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
        
        healthTextTransform.LookAt(mDestination);
        healthTextTransform.forward = -healthTextTransform.forward; // Invert
    }

    public void DoDamage(int dmg) {
        health -= dmg;

        healthText.text = health.ToString();
    }
    
    public void SetWave(Wave wave) {
        mWave = wave;
    }
}