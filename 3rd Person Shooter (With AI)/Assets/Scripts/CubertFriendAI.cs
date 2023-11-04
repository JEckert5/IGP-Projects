using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class CubertFriendAI : MonoBehaviour
{
    public Transform friend;
    private NavMeshAgent mAgent;

    // Start is called before the first frame update
    void Start() {
        mAgent = GetComponent<NavMeshAgent>();
    }

    // Update is called once per frame
    void Update()
    {
        //NavMeshAgent agent = GetComponent<NavMeshAgent>();
        mAgent.destination = friend.position;
    }
}
