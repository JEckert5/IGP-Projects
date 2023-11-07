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
    [SerializeField] private Canvas waveCanvas;
    private TextMeshProUGUI waveText;

    private void Start() {
        currentWave = waves[0];

        currentWave.Start();

        waveCanvas.enabled = true;
        waveText = waveCanvas.GetComponentInChildren<TextMeshProUGUI>();

        waveText.text = "Wave " + (waveCounter + 1);

        StartCoroutine(WaveTimer());
    }

    private void Update() {
        if (!currentWave.EveryoneIsDead()) return;

        waveCounter += 1;

        waveCanvas.enabled = true;
        waveText.text = "Wave " + (waveCounter + 1);

        StartCoroutine(WaveTimer());
        
        currentWave =  waves[waveCounter % waves.Length];
        currentWave.Start();
    }

    private IEnumerator WaveTimer() {
        yield return new WaitForSeconds(2.5f);

        waveCanvas.enabled = false;
    }
}
