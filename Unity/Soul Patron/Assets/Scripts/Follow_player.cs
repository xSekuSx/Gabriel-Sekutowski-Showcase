using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Follow_player : MonoBehaviour
{
    SphereCollider range_box;
    public GameObject enemy;
    void Start()
    {
        range_box = GetComponent<SphereCollider>();
    }
    private void OnTriggerEnter(Collider other) {
        enemy.GetComponent<Enemy_controller>().change_target(other.transform);
        range_box.enabled = false;
    }
}
