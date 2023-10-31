using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HealthText : MonoBehaviour {
    
    public Transform player;
    
    // Start is called before the first frame update
    void Start() {
        var t = GameObject.FindGameObjectWithTag("Player");

        player = t.transform;
    }

    // Update is called once per frame
    void Update() {
        Transform transform1;
        (transform1 = transform).LookAt(player);

        // Flip Text, forward vector is backwards technically
        transform1.forward = -transform1.forward;
    }
}
