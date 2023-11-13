using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class DoorButton : Interactable {

    private enum DoorState {
        Open,
        Closed,
    }

    [SerializeField] private Transform door;
    [SerializeField] private float doorRange;
    private Vector3 mDoorPos;
    private DoorState mDoorState = DoorState.Closed;
    private float mTiterator;
    private float mUpTarget;
    private float mDownTarget;
    private bool mOpen;

    private void Start() {
        mDoorPos = door.position;
        mUpTarget = mDoorPos.y + doorRange;
        mDownTarget = mDoorPos.y - doorRange;
    }

    public override void Action() {
        if (!active) return;
        
        switch (mDoorState) {
            case DoorState.Closed:
                OpenDoor();

                break;
            case DoorState.Open:
                CloseDoor();

                break;
        }
    }

    private void Update() {
        if (mDoorState == DoorState.Closed && )
    }

    private void OpenDoor() {
        while (mDoorPos.y < mUpTarget - 0.01f) {
            mDoorPos.y = Mathf.SmoothDamp(mDoorPos.y, mUpTarget, ref mTiterator, 0.5f);
            door.position = mDoorPos;
        }

        mTiterator = 0f;
        mDoorState = DoorState.Open;
    }

    private void CloseDoor() {
        while (mDoorPos.y > mDownTarget + 0.01f) {
            mDoorPos.y = Mathf.SmoothDamp(mDoorPos.y, mDownTarget, ref mTiterator, 0.5f);
            door.position = mDoorPos;
        }

        mTiterator = 0f;
        mDoorState = DoorState.Closed;
    }
}
