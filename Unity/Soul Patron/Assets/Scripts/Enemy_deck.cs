using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Enemy_deck : MonoBehaviour
{
    public List<Card> deck;
    public List<Card> create_enemy_deck() {
        Enemy enemy_component = this.GetComponent<Enemy>();
        for (int i = 2; i < 12; i++) {
            Card temp_card = new Card();
            temp_card.name = "Deal " + (i - 1) * 10 + " Damage";
            temp_card.value = (i - 1) * 10;
            temp_card.description = "The card deals " + (i - 1) * 10 + " Damage and costs " + Mathf.FloorToInt(i / 3) + " mana";
            temp_card.purpose = "Attack";
            temp_card.mana_cost = Mathf.FloorToInt(i / 3);
            if (i % 2 == 0) {
                temp_card.type = enemy_component.type1;
                temp_card.colour = enemy_component.type1_colour;
            }
            else {
                temp_card.type = enemy_component.type2;
                temp_card.colour = enemy_component.type2_colour;
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
                temp_card.type = enemy_component.type1;
                temp_card.colour = enemy_component.type1_colour;
            }
            else {
                temp_card.type = enemy_component.type2;
                temp_card.colour = enemy_component.type2_colour;
            }
            deck.Add(temp_card);
        }
        return deck;
    }
}
