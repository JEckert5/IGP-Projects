using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AmmoPickup : MonoBehaviour {
    [SerializeField] private int ammo;

    private Transform player;

    private float ypos;
    private float variance;
    private float yvel;
    private bool dir;

    private void Start() {
        player = GameObject.FindGameObjectWithTag("MainCamera").transform;
        
        if (player == null)
            Debug.Log("Scream and run");

        ypos = transform.position.y;
        variance = ypos + 0.5f;
    }

    private void Update() {
        transform.LookAt(player);
        transform.forward = -transform.forward;

        ypos = dir ? Mathf.SmoothDamp(ypos, variance, ref yvel, 0.4f) : Mathf.SmoothDamp(ypos, -variance, ref yvel, 0.4f);
        
        var position = transform.position;
        position = new Vector3(position.x, ypos, position.z);
        transform.position = position;

        dir = !(ypos >= variance);
    }

    private void OnTriggerEnter(Collider other) {
        if (!other.CompareTag("Player")) return;

        var s = other.gameObject.GetComponent<PlayerController>();

        s.currentReserveAmmo += ammo;
        s.reserveText.text = s.currentReserveAmmo.ToString();
        
        Destroy(gameObject); // Death
    }
}
