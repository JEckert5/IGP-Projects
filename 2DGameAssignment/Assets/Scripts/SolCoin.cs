using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SolCoin : MonoBehaviour {
    private Vector2 mScale;
    private Vector2 mVel;
    private bool mLerpDir;
    // Start is called before the first frame update
    void Start() {
        mScale = Vector2.one;
        mVel = Vector2.zero;
        mLerpDir = false;
    }

    // Update is called once per frame
    void Update() {
        if (mScale.x > 1.0f || mScale.y > 1.0f) {
            mLerpDir = false;
        }

        if (mScale.x < 0.7f || mScale.y < 0.7f)
            mLerpDir = true;

        if (mLerpDir) {
            mScale.x = Mathf.SmoothDamp(mScale.x, 1.01f, ref mVel.x, 0.5f);
            mScale.y = Mathf.SmoothDamp(mScale.y, 1.01f, ref mVel.y, 0.5f);
        }
        else {
            mScale.x = Mathf.SmoothDamp(mScale.x, 0.69f, ref mVel.x, 0.5f);
            mScale.y = Mathf.SmoothDamp(mScale.y, 0.69f, ref mVel.y, 0.5f);
        }

            transform.localScale = mScale;
    }
}
