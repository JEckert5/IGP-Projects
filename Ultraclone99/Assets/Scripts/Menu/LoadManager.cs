using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class LoadManager : MonoBehaviour
{
    // Start is called before the first frame update
    private void Start() {
        StartCoroutine(Load());
    }

    private static IEnumerator Load() {
        yield return new WaitForSecondsRealtime(3.5f);

        SceneManager.LoadScene("MainLevel");
    }
}
