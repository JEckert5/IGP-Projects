using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HealthText : MonoBehaviour {
    
    public Transform player;
    
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update() {
        // transform.position = poop.position + new Vector3(0, 1, 0);
        transform.LookAt(player);

        transform.forward = -transform.forward;
    }
}
