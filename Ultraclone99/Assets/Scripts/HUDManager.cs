using System;
using System.Collections;
using TMPro;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using UnityEngine.Video;

public class HUDManager : MonoBehaviour {
    [SerializeField] private Canvas hud;
    [SerializeField] private Canvas gameOver;
    [SerializeField] private VideoPlayer videoPlayer;
    [SerializeField] private RawImage mgsVid;
    [SerializeField] private TextMeshProUGUI objective;
    [SerializeField] private Image objectivePanel;
    private AudioSource mCar;

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

        if (wave == 2) {
            objective.text = "SPECIAL ROUND! KILL THE GERMAN!";
        } else
            objective.text = "Round " + wave;

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
        AudioManager.instance.Stop("BG");
        SceneManager.LoadScene("MainMenu");
    }

    public void GameOver() {
        Time.timeScale   = 0;
        AudioManager.instance.Stop("BG");

        var l = FindFirstObjectByType<VolksBoss>();
        
        if (l != null)
            l.Stop();
        
        if (Application.platform == RuntimePlatform.OSXEditor || Application.platform == RuntimePlatform.OSXPlayer)
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
