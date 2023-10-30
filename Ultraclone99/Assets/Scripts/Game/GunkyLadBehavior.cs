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

    // Start is called before the first frame update
    void Start() {
        mAgent = GetComponent<NavMeshAgent>();
        mAgent.destination = destination.position;
    }

    // Update is called once per frame
    void Update() {
        var objs = GameObject.FindGameObjectsWithTag("Player");
        
        if (objs.Length != 1)
            Debug.Log("FUCK");

        mAgent.destination = objs[0].transform.position;

        if (health <= 0) {
            DestroyImmediate(this);
        }

        healthText.text = health.ToString();
    }
}