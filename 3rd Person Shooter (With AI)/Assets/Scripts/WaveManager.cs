using System;
using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class WaveManager : MonoBehaviour {

    [SerializeField] private Wave[] waves;

    private Wave currentWave;
    private int waveCounter = 0;

    [SerializeField] private Canvas winCanvas;
    [SerializeField] private Canvas ammoCanvas;

    private void Start() {
        currentWave = waves[0];

        currentWave.Start();
    }

    private void Update() {
        if (!currentWave.EveryoneIsDead()) return;


        if (waveCounter >= waves.Length) return;
        
        waveCounter += 1;
        
        Debug.Log(waveCounter);

        if (waveCounter >= waves.Length) {
            Time.timeScale = 0f;

            ammoCanvas.enabled = false;
            winCanvas.enabled = true;

            Cursor.visible   = true;
            Cursor.lockState = CursorLockMode.None;
            
            var ot = winCanvas.GetComponentInChildren<TextMeshProUGUI>();
            ot.text = "YOU WIN!";
            
            return; // Win
        }
        
        currentWave =  waves[waveCounter];
        currentWave.Start();
        
        
    }
    
    
}
