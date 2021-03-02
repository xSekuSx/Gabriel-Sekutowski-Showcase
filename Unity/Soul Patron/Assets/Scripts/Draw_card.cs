using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Draw_card : MonoBehaviour
{
    [SerializeField] GameObject card_prefab;
    [SerializeField] GameObject right_hand;
    [SerializeField] GameObject left_hand;
    [SerializeField] GameObject combat;

    /*IEnumerator TimerRoutine() {
        //yield return new WaitForSeconds(5);
    }
    StartCoroutine(TimerRoutine());*/
    public void end_turn() {
        GetComponent<Button>().interactable = false;
        combat.GetComponent<Combat>().end_turn();
    }
    public void Draw_card_right() {
        if (Card_deck.right_deck.Count == 0) {
            return;
        }

        GameObject card = Instantiate<GameObject>(card_prefab);
        int number = Random.Range(0, Card_deck.right_deck.Count);
        
        card.transform.GetChild(1).GetComponent<Text>().text = Card_deck.right_deck[number].name;
        card.transform.GetChild(2).GetComponent<Text>().text = Card_deck.right_deck[number].description;
        card.GetComponent<Card>().left_right = "right";
        card.GetComponent<Card>().name = Card_deck.right_deck[number].name;
        card.GetComponent<Card>().description = Card_deck.right_deck[number].description;
        card.GetComponent<Card>().value = Card_deck.right_deck[number].value;
        card.GetComponent<Card>().purpose = Card_deck.right_deck[number].purpose;
        card.GetComponent<Card>().type = Card_deck.right_deck[number].type;
        card.GetComponent<Card>().colour = Card_deck.right_deck[number].colour;
        card.GetComponent<Card>().mana_cost = Card_deck.right_deck[number].mana_cost;
        Card_deck.right_deck.RemoveAt(number);
        card.GetComponent<Image>().color = (card.GetComponent<Card>().colour);
        if (card.GetComponent<Image>().color == new Color(1f, 1f, 1f, 1f) || card.GetComponent<Image>().color == new Color(160f / 255f, 1f, 0f, 1f)) {
            card.transform.GetChild(1).GetComponent<Text>().color = new Color(0f, 0f, 0f);
            card.transform.GetChild(2).GetComponent<Text>().color = new Color(0f, 0f, 0f);
        }
        card.transform.SetParent(right_hand.transform, false);
    }
    public void Draw_card_left() {
        if (Card_deck.left_deck.Count == 0) {
            return;
        }

        GameObject card = Instantiate<GameObject>(card_prefab);
        int number = Random.Range(0, Card_deck.left_deck.Count);

        card.transform.GetChild(1).GetComponent<Text>().text = Card_deck.left_deck[number].name;
        card.transform.GetChild(2).GetComponent<Text>().text = Card_deck.left_deck[number].description;
        card.GetComponent<Card>().left_right = "left";
        card.GetComponent<Card>().name = Card_deck.left_deck[number].name;
        card.GetComponent<Card>().description = Card_deck.left_deck[number].description;
        card.GetComponent<Card>().value = Card_deck.left_deck[number].value;
        card.GetComponent<Card>().purpose = Card_deck.left_deck[number].purpose;
        card.GetComponent<Card>().type = Card_deck.left_deck[number].type;
        card.GetComponent<Card>().colour = Card_deck.left_deck[number].colour;
        card.GetComponent<Card>().mana_cost = Card_deck.left_deck[number].mana_cost;
        Card_deck.left_deck.RemoveAt(number);
        card.GetComponent<Image>().color = (card.GetComponent<Card>().colour);
        if (card.GetComponent<Image>().color == new Color(1f, 1f, 1f, 1f) || card.GetComponent<Image>().color == new Color(160f / 255f, 1f, 0f, 1f)) {
            card.transform.GetChild(1).GetComponent<Text>().color = new Color(0f, 0f, 0f);
            card.transform.GetChild(2).GetComponent<Text>().color = new Color(0f, 0f, 0f);
        }
        card.transform.SetParent(left_hand.transform, false);
    }
    public void Draw_cards(string hand) {
        Draw_card_right();
        if (hand == "both") {
            Draw_card_left();
        }
        GetComponent<Button>().interactable = true;
    }
}
