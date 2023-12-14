using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Random = System.Random;

public class WaveManager : MonoBehaviour {

    [SerializeField] private Wave[] waves;
    [SerializeField] private Wave specialWave;
    [SerializeField] private Canvas victoryCanvas;
    [SerializeField] private Canvas game
    private Wave mCurrentWave;
    private int mRoundCounter = 1;
    private Random mRandom;
    
    private void Start() {
        mRandom = new Random();
        mCurrentWave = waves[mRandom.Next(waves.Length)];
        AudioManager.instance.Play("BG");
        victoryCanvas.enabled = false;
        
        mCurrentWave.Begin(this);
    }

    public void Signal(Wave wave) {
        if (wave == specialWave) {
            // Game Win!
            Time.timeScale = 0f;
            victoryCanvas.enabled = true;
        }
        
        mRoundCounter += 1;

        mCurrentWave = mRoundCounter == 7 ? specialWave : waves[mRandom.Next(waves.Length)];
        
        mCurrentWave.Begin(this);

        mGOS.NewRound(mRoundCounter);
    }
}
