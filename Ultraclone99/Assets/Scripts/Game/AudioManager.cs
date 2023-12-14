using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AudioManager : MonoBehaviour {
    public static AudioManager instance;
    
    public Sound[] sounds;
    private List<Sound> mPlaying = new List<Sound>();

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
            s.source.volume = s.volume;
        }
    }

    public void Play(string sound) {
        var s = Array.Find(sounds, item => item.name == sound);

        if (s == null) {
            Debug.Log("Sound " + sound + " not found.");
            return;
        }
        
        s.source.Play();
        mPlaying.Add(s);
    }

    public void Stop(string sound) {
        var s = mPlaying.Find(item => item.name == sound);

        if (s == null) {
            Debug.Log("Sound " + sound + " not found");
            return;
        }

        s.source.Stop();
        mPlaying.Remove(s);
    }
}
