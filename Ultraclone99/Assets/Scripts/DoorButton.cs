using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class DoorButton : Interactable {

    [SerializeField] private Animator animator;
    private bool mState;
    private readonly static int Open = Animator.StringToHash("Open"); // The overlords at JetBrains forced my hand

    private void Start() {
        mState = animator.GetBool(Open);
    }

    public override void Action() {
        base.Action();
        
        animator.SetBool(Open, !mState);
        mState = !mState;
    }

}
