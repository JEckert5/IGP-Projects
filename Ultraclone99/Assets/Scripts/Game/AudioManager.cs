using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AudioManager : MonoBehaviour {
    public static AudioManager instance;
    
    public Sound[] sounds;

    private void Awake() {
        if (instance != null)
            Destroy(gameObject);
        else {
            instance = this;
            DontDestroyOnLoad(gameObject);
        }

        foreach (var s in sounds) {
            s.source      = gameObject.AddComponent<AudioSource>();
            s.source.clip = s.clip;
            s.source.loop = s.loop;
        }
    }

    public void Play(string sound) {
        var s = Array.Find(sounds, item => item.name == sound);

        if (s == null) {
            Debug.Log("Sound " + name + " not found.");
            return;
        }
        
        s.source.Play();
    }
}
