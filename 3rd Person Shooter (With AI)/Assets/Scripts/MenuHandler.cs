using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using UnityEngine;
using UnityEngine.SceneManagement;

public class MenuHandler : MonoBehaviour {
    public void StartGame() {
        SceneManager.LoadScene("SampleScene");
    }
}
