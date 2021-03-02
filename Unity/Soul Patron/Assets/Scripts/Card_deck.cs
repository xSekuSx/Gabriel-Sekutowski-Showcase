using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class Card_deck : MonoBehaviour {
    static public List<Card> right_deck;
    static public List<Card> left_deck;
    static public List<Card> played_attack;
    static public List<Card> played_defence;
    Card temp_card;

    public List<Card> create_deck(Familiar familiar) {
        List<Card> deck = new List<Card>();
        for (int i = 2; i < 12; i++) {
            Card temp_card = new Card();
            temp_card.name = "Deal " + (i - 1) * 10 + " Damage";
            temp_card.value = (i - 1) * 10;
            temp_card.description = "The card deals " + (i - 1) * 10 + " Damage and costs " + Mathf.FloorToInt(i / 3) + " mana";
            temp_card.purpose = "Attack";
            temp_card.mana_cost = Mathf.FloorToInt(i / 3);
            if (i % 2 == 0) {
                temp_card.type = familiar.type1;
                temp_card.colour = familiar.type1_colour;
            }
            else {
                temp_card.type = familiar.type2;
                temp_card.colour = familiar.type2_colour;
            }
            deck.Add(temp_card);
        }
        for (int i = 2; i < 6; i++) {
            Card temp_card = new Card();
            temp_card.name = "Block " + ((i - 1) * 10) / 2 + " Damage";
            temp_card.value = ((i - 1) * 10) / 2;
            temp_card.description = "The card blocks " + ((i - 1) * 10) / 2 + " Damage and costs " + Mathf.FloorToInt(i / 3) + " mana";
            temp_card.purpose = "Defence";
            temp_card.mana_cost = Mathf.FloorToInt(i / 3);
            if (i % 2 == 0) {
                temp_card.type = familiar.type1;
                temp_card.colour = familiar.type1_colour;
            }
            else {
                temp_card.type = familiar.type2;
                temp_card.colour = familiar.type2_colour;
            }
            deck.Add(temp_card);
        }
        return deck;
    }
    public void create_decks(GameObject right_familiar, GameObject left_familiar) {
        right_deck = create_deck(right_familiar.GetComponent<Familiar>());
        if (left_familiar != null) {
            left_deck = create_deck(left_familiar.GetComponent<Familiar>());
        }
    }
    public List<Card> get_right_deck() {
        return right_deck;
    }
    public List<Card> get_left_deck() {
        return left_deck;
    }
}
