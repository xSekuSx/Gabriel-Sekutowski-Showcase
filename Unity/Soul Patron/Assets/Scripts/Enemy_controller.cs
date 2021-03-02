using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class Enemy_controller: MonoBehaviour {
    public Transform destination1;
    public Transform destination2;
    public NavMeshAgent agent;

    Transform target;

    CapsuleCollider hit_box;
    public SphereCollider range_box;

    // Start is called before the first frame update
    void Start() {
        agent = GetComponent<NavMeshAgent>();
        target = destination1;
        hit_box = GetComponent<CapsuleCollider>();
    }

    // Update is called once per frame
    void Update() {
        agent.destination = target.position;
        if (Vector3.Distance(agent.transform.position, target.position) < 1f) {
            if (target == destination1) {
                target = destination2;
            }
            else {
                target = destination1;
            }
        }
    }
    public void change_target(Transform new_target) {
        target = new_target;
    }
}