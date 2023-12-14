using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class LoadManager : MonoBehaviour
{
    // Start is called before the first frame update
    private void Start() {
        AudioManager.instance.Play("LoadingMusic");
        StartCoroutine(Load());
    }

    private static IEnumerator Load() {
        yield return new WaitForSecondsRealtime(6f);

        AudioManager.instance.Stop("LoadingMusic");
        SceneManager.LoadScene("MainLevel");
    }
}
