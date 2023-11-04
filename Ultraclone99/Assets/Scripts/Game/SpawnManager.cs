using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpawnManager : MonoBehaviour {

    public GameObject gunkyPrefab;
    
    private List<GunkyLadBehavior> mLads;
    
    // Start is called before the first frame update
    void Start() {
        mLads           = new List<GunkyLadBehavior>();
    }

    // Update is called once per frame
    void Update() {
        while (mLads.Count < 4) {
            var go = Instantiate(gunkyPrefab, transform.position, Quaternion.identity);
            var lad  = go.GetComponentInChildren<GunkyLadBehavior>();
            lad.SetSpawnManager(this);
            
            // mSpawnedObjects.Add(test);
            mLads.Add(lad);
        }
    }

    public void KillMe(GunkyLadBehavior ladToDestroy) {
        mLads.Remove(ladToDestroy);
        
        Destroy(ladToDestroy.gameObject);
    }
}
