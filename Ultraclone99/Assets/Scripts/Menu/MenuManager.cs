using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class MenuManager : MonoBehaviour
{
    private void Start() {
        AudioManager.instance.Play("MenuMusic");
    }

    public void StartGame() {
        SceneManager.LoadScene("LoadingScreen");
    }

    public void Exit() {
        Application.Quit();
    }
}
