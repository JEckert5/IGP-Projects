using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AmmoPickup : MonoBehaviour {
    [SerializeField] private int ammo;

    private Transform player;

    private void Start() {
        player = GameObject.FindGameObjectWithTag("Player").transform;
        
        if (player == null)
            Debug.Log("Scream and run");
    }

    private void Update() {
        transform.LookAt(player);
    }

    private void OnTriggerEnter(Collider other) {
        if (!other.CompareTag("Player")) return;

        var s = other.gameObject.GetComponent<PlayerController>();

        s.currentReserveAmmo += ammo;
        s.reserveText.text = s.currentReserveAmmo.ToString();
        
        Destroy(gameObject); // Death
    }
}
