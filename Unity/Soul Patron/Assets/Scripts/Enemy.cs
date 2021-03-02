using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Enemy : MonoBehaviour
{
    public int HP = 100;
    public int current_mana = 0;
    public int max_mana = 0;
    public string type1;
    public Color type1_colour;
    public string type2;
    public Color type2_colour;
    public Text health_text;
    public Text mana_text;
    public GameObject attacking_zone;
    public GameObject deffending_zone;
    public GameObject card_prefab;
    GameObject card_to_place;
    List<Card> enemy_deck;
    List<Card> hand = new List<Card>();
    GameObject current_card_object;

    void Start() {
        enemy_deck = GetComponent<Enemy_deck>().create_enemy_deck();
    }

    void create_card(Card card_played, Transform zone) {

        GameObject card = Instantiate<GameObject>(card_prefab);

        card.transform.GetChild(1).GetComponent<Text>().text = card_played.name;
        card.transform.GetChild(2).GetComponent<Text>().text = card_played.description;
        card.GetComponent<Card>().name = card_played.name;
        card.GetComponent<Card>().description = card_played.description;
        card.GetComponent<Card>().value = card_played.value;
        card.GetComponent<Card>().purpose = card_played.purpose;
        card.GetComponent<Card>().type = card_played.type;
        card.GetComponent<Card>().colour = card_played.colour;
        card.GetComponent<Card>().mana_cost = card_played.mana_cost;
        card.GetComponent<Image>().color = (card_played.colour);
        if (card.GetComponent<Image>().color == new Color(1f, 1f, 1f, 1f)) {
            card.transform.GetChild(1).GetComponent<Text>().color = new Color(0f, 0f, 0f);
            card.transform.GetChild(2).GetComponent<Text>().color = new Color(0f, 0f, 0f);
        }
        card.transform.SetParent(zone, false);
    }
    public IEnumerator show_attack(Combat combat_controller, GameObject UI, GameObject side_right_camera, GameObject top_down_camera) {
        for (int i = 0; i < attacking_zone.transform.childCount; i++) {
            current_card_object = attacking_zone.transform.GetChild(i).gameObject;
            if (current_card_object.GetComponent<Card>() != null) {
                top_down_camera.SetActive(false);
                side_right_camera.SetActive(true);
                var main = GetComponent<ParticleSystem>().main;
                main.startColor = current_card_object.GetComponent<Card>().colour;
                GetComponent<ParticleSystem>().Play();
                yield return new WaitForSeconds(4);
                side_right_camera.SetActive(false);
                top_down_camera.SetActive(true);
                combat_controller.take_damage(current_card_object.GetComponent<Card>().value, current_card_object.GetComponent<Card>().type);
                Destroy(current_card_object);
            }
        }
        UI.SetActive(true);
    }

    public void place_cards(bool attacking_placement) {
        for (int i = 0; i < hand.Count; i++) {
            if (hand[i].purpose == "Attack" && attacking_placement) {
                if (hand[i].mana_cost <= current_mana) {
                    current_mana -= hand[i].mana_cost;
                    mana_text.text = "Mana: " + current_mana;
                    create_card(hand[i], attacking_zone.transform);
                    hand.RemoveAt(i);
                    i--;
                }
            }
            else if (hand[i].purpose == "Defence" && !attacking_placement) {
                if (hand[i].mana_cost <= current_mana) {
                    current_mana -= hand[i].mana_cost;
                    mana_text.text = "Mana: " + current_mana;
                    create_card(hand[i], deffending_zone.transform);
                    hand.RemoveAt(i);
                    i--;
                }
            }
        }
    }

    public void draw_card() {
        if (enemy_deck.Count == 0) {
            return;
        }
        int number = Random.Range(0, enemy_deck.Count);
        hand.Add(enemy_deck[number]);
        enemy_deck.RemoveAt(number);
    }

    public bool take_damage(int damage, string type) {
        if (damage == 0) {
            health_text.text = "Health:\n" + HP;
            return false;
        }
        float modifier = 1;
        if (type1 == "Dark" || type2 == "Dark") {
            if (type == "Light") {
                modifier *= 2;
            }
            else if (type == "Fire") {
                modifier *= 2;
            }
            else if(type == "Electricity") {
                modifier /= 2;
            }
            else if(type == "Wood") {
                modifier /= 2;
            }
        }
        if (type1 == "Earth" || type2 == "Earth") {
            if(type == "Light") {
                modifier *= 2;
            }
            else if(type == "Wood") {
                modifier *= 2;
            }
            else if(type == "Water") {
                modifier /= 2;
            }
            else if(type == "Air") {
                modifier /= 2;
            }
        }
        if (type1 == "Ice" || type2 == "Ice") {
            if (type == "Air") {
                modifier *= 2;
            }
            else if (type == "Wood") {
                modifier *= 2;
            }
            else if (type == "Light") {
                modifier /= 2;
            }
            else if (type == "Water") {
                modifier /= 2;
            }
        }
        if (type1 == "Metal" || type2 == "Metal") {
            if (type == "Fire") {
                modifier *= 2;
            }
            else if (type == "Electricity") {
                modifier *= 2;
            }
            else if (type == "Water") {
                modifier /= 2;
            }
            else if (type == "Air") {
                modifier /= 2;
            }
        }
        HP = HP - Mathf.RoundToInt(damage * modifier);
        health_text.text = "Health:\n" + HP;
        if (HP <= 0) {
            foreach (Transform child in attacking_zone.transform) {
                GameObject.Destroy(child.gameObject);
            }
            foreach (Transform child in deffending_zone.transform) {
                GameObject.Destroy(child.gameObject);
            }
            return true;
        }
        return false;
    }
}
