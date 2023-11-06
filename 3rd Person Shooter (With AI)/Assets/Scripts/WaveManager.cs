using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WaveManager : MonoBehaviour {

    [SerializeField] private Wave[] waves;

    private Wave currentWave;
    private int waveCounter = 0;

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
            return; // Win
        }
        
        currentWave =  waves[waveCounter];
        currentWave.Start();
        
        
    }
    
    
}
