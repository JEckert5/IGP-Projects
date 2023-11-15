using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WaveManager : MonoBehaviour {

    [SerializeField] private Wave[] waves;
    private Wave mCurrentWave;
    private int mWaveCounter; 
    
    
    // Start is called before the first frame update
    void Start() {
        mCurrentWave = waves[mWaveCounter];

        mWaveCounter += 1;
        mCurrentWave.Begin(this);
    }

    public void Signal(Wave wave) {
        if (mWaveCounter >= waves.Length) return;
        
        mCurrentWave =  waves[mWaveCounter];
        
        mWaveCounter += 1;
        
        mCurrentWave.Begin(this);
    }
}
