using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEditor;
using UnityEngine;
using Random = System.Random;

[CreateAssetMenu(menuName = "Wave", fileName = "wave")]
public class Wave : ScriptableObject {

    public GameObject enemyPrefab;
    public int size;
    public string locationTag;
    
    private List<GameObject> livingDudes;

    public bool EveryoneIsDead() {
        return livingDudes.Count <= 0;
    }

    public void Start() {
        Random r = new Random();
        livingDudes = new List<GameObject>();

        var go   = GameObject.FindGameObjectWithTag(locationTag);
        var locs = go.GetComponentsInChildren<Transform>();
        
        for (int i = 0; i < size; i++) {
           // Debug.Log(i + locs[i].tag);
            var dude = Instantiate(enemyPrefab, locs[r.Next(1, locs.Length)].position, Quaternion.identity);
            var dudeController = dude.GetComponent<EnemyController>();
            
            dudeController.SetWave(this);
            livingDudes.Add(dude);
        }
    }

    public void Signal(EnemyController deadDude) {
        livingDudes.Remove(deadDude.gameObject);
    }

}
