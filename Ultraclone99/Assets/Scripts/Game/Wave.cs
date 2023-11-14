using System.Collections.Generic;
using Random = System.Random;
using UnityEngine;

[CreateAssetMenu(menuName = "Wave", fileName = "wave")]
public class Wave: ScriptableObject {

    [SerializeField] private GameObject prefab;
    [SerializeField] private string spawnTag;
    [SerializeField] private int size;
    private List<Transform> mSpawnPoints;
    private List<GunkyLadController> mDudes;
    private WaveManager mParent;
    
    public void Begin(WaveManager wm) {
        mParent = wm;
        mDudes  = new List<GunkyLadController>();
        var p = GameObject.FindGameObjectWithTag(spawnTag).GetComponentsInChildren<Transform>();

        mSpawnPoints = new List<Transform>(p);

        mSpawnPoints.Remove(p[0]);

        Random random = new Random();
        
        for (int i = 0; i < size; i++) {
            var sp  = mSpawnPoints[random.Next(mSpawnPoints.Count)];
            var lad = Instantiate(prefab, sp.position, Quaternion.identity).GetComponentInChildren<GunkyLadController>();

            lad.SetWave(this);
            mDudes.Add(lad);
        }
    }

    public void Signal(GunkyLadController lad) {
        mDudes.Remove(lad);
        Destroy(lad.gameObject);

        if (mDudes.Count == 0)
            mParent.Signal(this);
    }

    public bool Over() {
        return mDudes.Count == 0;
    }
}
