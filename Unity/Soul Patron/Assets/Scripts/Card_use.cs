using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class Card_use : MonoBehaviour, IBeginDragHandler, IDragHandler, IEndDragHandler
{
    public Transform parent_to_return = null;

    [SerializeField]
    Camera camera;
    GameObject combat;

    GameObject place_holder;
    
    public void OnBeginDrag(PointerEventData event_data) {
        parent_to_return = this.transform.parent;

        place_holder = new GameObject();
        place_holder.transform.SetParent(parent_to_return);
        LayoutElement le = place_holder.AddComponent<LayoutElement>();
        le.preferredHeight = this.GetComponent<LayoutElement>().preferredHeight;
        le.preferredWidth = this.GetComponent<LayoutElement>().preferredWidth;
        place_holder.transform.SetSiblingIndex(this.transform.GetSiblingIndex());

        this.transform.SetParent(parent_to_return.parent.transform);
        GetComponent<CanvasGroup>().blocksRaycasts = false;
    }
    public void OnDrag(PointerEventData event_data) {
        this.transform.position = event_data.position;

        int new_sibling_index = parent_to_return.childCount;
        for (int i = 0; i < parent_to_return.childCount; i++) {
            if (this.transform.position.x < parent_to_return.GetChild(i).position.x) {
                new_sibling_index = i;
                if (place_holder.transform.GetSiblingIndex() < new_sibling_index) {
                    new_sibling_index--;
                }
                break;
            }
        }
        place_holder.transform.SetSiblingIndex(new_sibling_index);
    }
    public void OnEndDrag(PointerEventData event_data) {

        RaycastHit hit;
        Ray ray = camera.ScreenPointToRay(Input.mousePosition);

        if (Physics.Raycast(ray, out hit)) {
            Drop_zone d = hit.transform.gameObject.GetComponent<Drop_zone>();
            if (d != null){
                if (d.field == this.GetComponent<Card>().purpose) {
                    if (this.GetComponent<Card>().left_right == "left") {
                        if (Combat.left_mana >= this.GetComponent<Card>().mana_cost) {
                            Combat.left_mana -= this.GetComponent<Card>().mana_cost;
                            d.Drop(this.gameObject);
                            Destroy(place_holder);
                            return;
                        }
                    }
                    else if (this.GetComponent<Card>().left_right == "right") {
                        if (Combat.right_mana >= this.GetComponent<Card>().mana_cost) {
                            Combat.right_mana -= this.GetComponent<Card>().mana_cost;
                            d.Drop(this.gameObject);
                            Destroy(place_holder);
                            return;
                        }
                    }
                }
            }
        }

        GetComponent<CanvasGroup>().blocksRaycasts = true;
        this.transform.SetParent(parent_to_return);
        this.transform.SetSiblingIndex(place_holder.transform.GetSiblingIndex());
        Destroy(place_holder);
    }
}
