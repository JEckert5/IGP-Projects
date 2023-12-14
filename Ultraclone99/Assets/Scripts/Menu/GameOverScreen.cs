using System;
using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using UnityEngine.Video;

public class GameOverScreen : MonoBehaviour {
    [SerializeField] private Canvas hud;
    [SerializeField] private Canvas gameOver;
    [SerializeField] private VideoPlayer videoPlayer;
    [SerializeField] private RawImage mgsVid;
    [SerializeField] private RawImage mgs;
    [SerializeField] private TextMeshProUGUI objective;
    [SerializeField] private Image objectivePanel;

    private void Start() {
        gameOver.enabled = false;

        StartCoroutine(ObjectiveFade());
    }

    private IEnumerator ObjectiveFade() {
        yield return new WaitForSecondsRealtime(5f);

        objective.enabled = false;
        objectivePanel.enabled = false;
    }

    public void NewRound(int wave) {
        objective.enabled = true;
        objectivePanel.enabled = true;
        objective.text = "Round " + wave.ToString();

        StartCoroutine(NewFade());
    }

    private IEnumerator NewFade() {
        yield return new WaitForSecondsRealtime(5f);

        objective.enabled = false;
        objectivePanel.enabled = false;
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
        AudioManager.instance.Stop("BG");
        AudioManager.instance.Play("DeathSound");
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
