using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HealthText : MonoBehaviour {
    
    private Transform mPlayer;
    
    // Start is called before the first frame update
    void Start() {
        var t = GameObject.FindGameObjectWithTag("Player");

        mPlayer = t.transform;
    }

    // Update is called once per frame
    void Update() {
        transform.LookAt(mPlayer);

        // Flip Text, forward vector is backwards technically
        transform.forward *= -1;
    }
}
