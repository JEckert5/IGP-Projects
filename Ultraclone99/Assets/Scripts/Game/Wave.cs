

using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(menuName = "Wave", fileName = "wave")]
public class Wave: ScriptableObject {

    [SerializeField] private GameObject prefab;
    [SerializeField] private string spawnTag;
    private List<Transform> mSpawnPoints;
    private List<GunkyLadBehavior> mDudes;
    

    public void Start() {
        mDudes = new List<GunkyLadBehavior>();
        var p = GameObject.FindGameObjectWithTag(spawnTag).GetComponentsInChildren<Transform>();
        
        Debug.Log(p[0].tag);

        mSpawnPoints = new List<Transform>(p);

        mSpawnPoints.Remove(p[0]);
    }

}
