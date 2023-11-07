using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WaveManager : MonoBehaviour {

    [SerializeField] private Wave[] waves;
    private Wave mCurrentWave;
    private int mWaveCounter = 0;
    
    
    // Start is called before the first frame update
    void Start() {
        mCurrentWave = waves[mWaveCounter];

        mWaveCounter += 1;
        mCurrentWave.Begin(this);
    }

    // Update is called once per frame
    void Update() {
        
    }

    public void Signal(Wave wave) {
        mCurrentWave =  waves[mWaveCounter];
        
        mWaveCounter += 1;
        
        mCurrentWave.Begin(this);
    }
}
