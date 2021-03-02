using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public class Interactable : MonoBehaviour
{
    public bool in_range;
    public KeyCode interaction_key;
    public GameObject new_familiar;
    public UnityEvent interaction;

    private void OnTriggerEnter(Collider other) {
        if (other.tag == "Player") {
            in_range = true;
        }
    }

    private void OnTriggerExit(Collider other) {
        if (other.tag == "Player") {
            in_range = false;
        }
    }

    void Update() {
        if (in_range) {
            if (Input.GetKeyDown(interaction_key)) {
                interaction.Invoke();
                Destroy(this.gameObject);
            }
        }
    }

    public void add_familiar(GameObject player) {
        GameObject fam = Instantiate<GameObject>(new_familiar);
        fam.transform.position = fam.transform.position + player.transform.position;
        fam.transform.SetParent(player.transform);
    }

    public void collect() {
        Destroy(this.gameObject);
    }

}
