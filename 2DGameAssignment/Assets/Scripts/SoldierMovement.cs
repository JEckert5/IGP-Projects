using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SoldierMovement : MonoBehaviour {
    
    private Rigidbody2D mSoldier;

    private Animator mAnim;

    private Vector3 mPos;
    // Start is called before the first frame update
    void Start() {
        mSoldier = GetComponent<Rigidbody2D>();
        mAnim    = GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
