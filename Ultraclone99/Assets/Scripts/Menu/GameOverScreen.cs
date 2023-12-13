using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using UnityEngine.Video;

public class GameOverScreen : MonoBehaviour {
    [SerializeField] private Canvas hud;
    [SerializeField] private Canvas gameOver;
    [SerializeField] private VideoPlayer videoPlayer;
    [SerializeField] private RawImage mgsVid;
    [SerializeField] private RawImage mgs;

    private void Start() {
        gameOver.enabled = false;
    }

    public void Retry() {
        Time.timeScale = 1;
        SceneManager.LoadScene("MainLevel");
    }

    public void Quit() {
        Time.timeScale = 1;
        SceneManager.LoadScene("MainMenu");
    }

    public void GameOver() {
        Time.timeScale   = 0;
        hud.enabled      = false;
        gameOver.enabled = true;
        videoPlayer.Play();
        Cursor.visible   = true;
        Cursor.lockState = CursorLockMode.None;
        StartCoroutine(WaitForVideo());
    }

    private IEnumerator WaitForVideo() {
        yield return new WaitForSecondsRealtime(8.5f);
        
        videoPlayer.Stop();
        videoPlayer.enabled = false;
        mgsVid.enabled      = false;
    }
}
