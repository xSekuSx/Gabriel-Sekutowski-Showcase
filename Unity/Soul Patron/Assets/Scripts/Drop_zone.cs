using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class Drop_zone : MonoBehaviour
{
    [SerializeField] GameObject card_prefab;
    public string field;
    public GameObject player;
    public void Drop(GameObject card) {
        card.transform.rotation = Quaternion.Euler(0f, 0f, 0f);
        card.transform.SetParent(this.transform,false);
        player.GetComponent<Combat>().update_mana();
    }
}
