using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpawnManager : MonoBehaviour {

    public GameObject gunkyPrefab;

    private List<GameObject> mSpawnedObjects;
    
    // Start is called before the first frame update
    void Start() {
        mSpawnedObjects = new List<GameObject>();
    }

    // Update is called once per frame
    void Update()
    {
        while (mSpawnedObjects.Count < 4) {
            var test = Instantiate(gunkyPrefab, transform.position, Quaternion.identity);
            
            mSpawnedObjects.Add(test);
        }
        
        foreach (var e in mSpawnedObjects) {
            var g = e.GetComponentInChildren<GunkyLadBehavior>();
                
            Debug.Log(g.ShouldBeDestroyed());

            if (g.ShouldBeDestroyed()) {
                Debug.Log("destroy");
                mSpawnedObjects.Remove(e);
                Destroy(e);
            }
        }
    }
}
