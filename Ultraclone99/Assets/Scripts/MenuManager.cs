using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class MenuManager : MonoBehaviour {
    [SerializeField] private Canvas credits;
    [SerializeField] private Canvas mainCanvas;
    
    private void Start() {
        AudioManager.instance.Play("MenuMusic");
        credits.enabled = false;
    }

    public void StartGame() {
        AudioManager.instance.Stop("MenuMusic");
        SceneManager.LoadScene("LoadingScreen");
    }

    public void Credits() {
        credits.enabled = true;
        mainCanvas.enabled = false;
    }

    public void BackFromCredits() {
        credits.enabled = false;
        mainCanvas.enabled = true;
    }

    public void Exit() {
        Application.Quit();
    }
}
