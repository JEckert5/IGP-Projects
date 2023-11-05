using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using UnityEngine;
using UnityEngine.SceneManagement;

public class MenuHandler : MonoBehaviour {

    public Canvas menuCanvas;
    public Canvas htpCanvas;

    private void Awake() {
        menuCanvas.enabled = true;
        htpCanvas.enabled = false;
    }
    
    public void StartGame() {
        SceneManager.LoadScene("SampleScene");
    }

    public void HowToPlay() {
        menuCanvas.enabled = false;
        htpCanvas.enabled = true;
    }

    public void Back() {
        menuCanvas.enabled = true;
        htpCanvas.enabled = false;
    }
}
