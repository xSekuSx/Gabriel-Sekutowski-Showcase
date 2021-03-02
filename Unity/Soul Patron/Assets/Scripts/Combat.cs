using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class Combat : MonoBehaviour {
    public Transform p1_postion;
    public Transform p2_postion;
    public Transform f1_postion;
    public Transform f2_postion;
    public GameObject player_camera;
    public GameObject top_down_camera;
    public GameObject side_right_camera;
    public GameObject side_left_camera;
    public GameObject familiar_left;
    public GameObject familiar_right;
    public GameObject left_hand;
    public GameObject right_hand;
    public GameObject draw_card;
    public List<Card> right_deck;
    public List<Card> left_deck;
    public GameObject played_attack;
    public GameObject played_defence;
    [SerializeField] GameObject loot;
    Vector3 drop_location;
    GameObject enemy;
    public Text HP_text;
    public Text Mana_text;
    int player_HP;
    int right_HP;
    int left_HP;
    public static int left_mana = 1;
    public static int right_mana = 1;
    public static int player_max_mana = 1;
    bool player_attacker = true;

    public GameObject UI;
    Vector3 location;
    Vector3 familiar_location;
    Vector3 familiar2_location;

    private void OnTriggerEnter(Collider other) {
        if (other.tag == "Enemy") {
            enemy = other.gameObject;
            drop_location = enemy.transform.position;
            engage_combat();
        }
    }

    private void engage_combat() {
        left_mana = 1;
        right_mana = 1;
        player_max_mana = 1;
        this.GetComponent<Animator>().SetBool("Fighting",true);
        this.GetComponent<Animator>().SetInteger("Direction", -1);
        this.transform.rotation = Quaternion.identity;
        enemy.transform.rotation = Quaternion.Euler(0f, 180f, 0f);
        location = this.transform.position;
        familiar_location = familiar_right.transform.position;
        GetComponent<Player_movement>().enabled = false;
        player_camera.SetActive(false);
        top_down_camera.SetActive(true);
        Cursor.lockState = CursorLockMode.None;
        enemy.GetComponent<Enemy_controller>().enabled = false;
        enemy.GetComponent<UnityEngine.AI.NavMeshAgent>().enabled = false;
        this.transform.position = p2_postion.position;
        enemy.transform.position = p1_postion.position;
        if (this.transform.GetChild(this.transform.childCount - 1).gameObject.tag == "Familiar") {
            familiar_left = this.transform.GetChild(this.transform.childCount - 1).gameObject;
            familiar2_location = familiar_left.transform.position;
            familiar_left.transform.position = f1_postion.position;
            familiar_left.transform.LookAt(enemy.transform);
        }
        familiar_right.transform.position = f2_postion.position;
        familiar_right.transform.LookAt(enemy.transform);
        UI.SetActive(true);
        this.GetComponent<Change_World>().change_combat_state();
        start_battle();
    }
    private void start_battle() {
        GetComponent<Card_deck>().create_decks(familiar_right, familiar_left);
        right_deck = GetComponent<Card_deck>().get_right_deck();
        left_deck = GetComponent<Card_deck>().get_left_deck();
        if (left_deck == null) {
            left_hand.SetActive(false);
        }
        right_HP = familiar_right.GetComponent<Familiar>().HP;
        if (familiar_left != null) {
            left_HP = familiar_left.GetComponent<Familiar>().HP;
            player_HP = left_HP + right_HP;
            update_HP();
        }
        else {
            player_HP = right_HP;
            update_HP();
        }
        enemy.GetComponent<Enemy>().take_damage(0, "None");
        draw_cards(4, true);
        draw_cards(4, false);
        update_mana();
    }

    private void draw_cards(int number, bool player) {
        if (player) {
            if (left_deck == null) {
                for (int i = 0; i < number; i++) {
                    draw_card.GetComponent<Draw_card>().Draw_cards("right");
                }
            }
            else {
                for (int i = 0; i < number; i++) {
                    draw_card.GetComponent<Draw_card>().Draw_cards("both");
                }
            }
        }
        else {
            for (int i = 0; i < number; i++) {
                enemy.GetComponent<Enemy>().draw_card();
            }
        }
    }

    private void update_HP() {
        string message = "Health:\n";
        if (familiar_left != null) {
            if ((left_HP / 10) >= 10) {
                message += left_HP + " / ";
            }
            else if ((left_HP / 10) >= 1) {
                message += "0" + left_HP + " / ";
            }
            else {
                message += "00" + left_HP + " / ";
            }
        }
        else {message += "000 / ";
        }
        if ((right_HP / 10) >= 10) {
            message += right_HP;
        }
        else if ((right_HP / 10) >= 1) {
            message += "0" + right_HP;
        }
        else {
            message += "00" + right_HP;
        }
        HP_text.text = message;
    }

    public void update_mana() {
        string message = "Mana:";
        message += " " + left_mana + "/" + right_mana;
        Mana_text.text = message;
    }

    int damage_to_familiar(int damage, string type, GameObject familiar) {
        string type1 = familiar.GetComponent<Familiar>().type1;
        string type2 = familiar.GetComponent<Familiar>().type2;
        float modifier = 1;
        if (type1 == "Dark" || type2 == "Dark") {
            if (type == "Light") {
                modifier *= 2;
            }
            else if (type == "Fire") {
                modifier *= 2;
            }
            else if (type == "Electricity") {
                modifier /= 2;
            }
            else if (type == "Wood") {
                modifier /= 2;
            }
        }
        if (type1 == "Earth" || type2 == "Earth") {
            if (type == "Light") {
                modifier *= 2;
            }
            else if (type == "Wood") {
                modifier *= 2;
            }
            else if (type == "Water") {
                modifier /= 2;
            }
            else if (type == "Air") {
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
        if (type1 == "Light" || type2 == "Light") {
            if (type == "Earth") {
                modifier *= 2;
            }
            else if (type == "Dark") {
                modifier *= 2;
            }
            else if (type == "Metal") {
                modifier /= 2;
            }
            else if (type == "Ice") {
                modifier /= 2;
            }
        }
        if (type1 == "Water" || type2 == "Water") {
            if (type == "Ice") {
                modifier *= 2;
            }
            else if (type == "Earth") {
                modifier *= 2;
            }
            else if (type == "Fire") {
                modifier /= 2;
            }
            else if (type == "Electricity") {
                modifier /= 2;
            }
        }
        return Mathf.RoundToInt(damage * modifier);                
    }

    public void take_damage(int damage, string type) {
        int left_damage = damage / 2;
        int right_damage = damage / 2;
        if (familiar_left == null) {
            right_damage *= 2;
            damage = damage_to_familiar(right_damage, type, familiar_right);
            right_HP -= damage;
            if (right_HP <= 0) {
                right_HP = 0;
            }
            player_HP = right_HP;
            if (player_HP == 0) {
                SceneManager.LoadScene("Demo");
            }
            update_HP();
            return;
        }
        left_damage = damage_to_familiar(left_damage, type, familiar_left);
        right_damage = damage_to_familiar(right_damage, type, familiar_right);
        left_HP -= left_damage;
        right_HP -= right_damage;
        if (left_HP <= 0) {
            left_HP = 0;
        }
        if (right_HP <= 0) {
            right_HP = 0;
        }
        player_HP = left_HP + right_HP;
        if (player_HP == 0) {
            SceneManager.LoadScene("Demo");
        }
        update_HP();
    }

    public void end_turn() {
        UI.SetActive(false);
        draw_cards(1, true);
        draw_cards(1, false);
        enemy.GetComponent<Enemy>().draw_card();
        if (player_attacker) {
            enemy.GetComponent<Enemy>().place_cards(false);
            attack();
            player_attacker = !player_attacker;
            left_mana = player_max_mana;
            right_mana = player_max_mana;
            update_mana();
            enemy.GetComponent<Enemy>().place_cards(true);
        }
        else {
            block();
            player_attacker = !player_attacker;
            enemy.GetComponent<Enemy>().max_mana++;
            enemy.GetComponent<Enemy>().current_mana = enemy.GetComponent<Enemy>().max_mana;
            enemy.GetComponent<Enemy>().mana_text.text = "Mana: " + enemy.GetComponent<Enemy>().max_mana;
            player_max_mana++;
            left_mana = player_max_mana;
            right_mana = player_max_mana;
            update_mana();
        }
    }

    IEnumerator show_attack() {
        foreach (Transform child in played_attack.transform) {
            GameObject child_game_object = child.gameObject;
            Card child_card = child_game_object.GetComponent<Card>();
            top_down_camera.SetActive(false);
            side_left_camera.SetActive(true);
            if (child_card.left_right == "right") {
                var main = familiar_right.GetComponentInChildren<ParticleSystem>().main;
                main.startColor = child_game_object.GetComponent<Card>().colour;
                familiar_right.GetComponentInChildren<ParticleSystem>().Play();
            }
            else {
                var main = familiar_left.GetComponentInChildren<ParticleSystem>().main;
                main.startColor = child_game_object.GetComponent<Card>().colour;
                familiar_left.GetComponentInChildren<ParticleSystem>().Play();
            }
            yield return new WaitForSeconds(4);
            side_left_camera.SetActive(false);
            top_down_camera.SetActive(true);
            bool dead = enemy.GetComponent<Enemy>().take_damage(child_card.value, child_card.type);
            if (dead) {
                end_battle();
                yield break;
            }
            Destroy(child_game_object);
        }
        UI.SetActive(true);
    }

    private void attack() {
        StartCoroutine(show_attack());
    }

    private void block() {
        StartCoroutine(enemy.GetComponent<Enemy>().show_attack(this.GetComponent<Combat>(), UI, side_right_camera, top_down_camera));
    }

    private void clear_table(GameObject zone) {
        foreach (Transform child in zone.transform) {
            GameObject.Destroy(child.gameObject);
        }
    }

    private void end_battle() {
        clear_table(played_attack);
        clear_table(played_defence);
        clear_table(left_hand);
        clear_table(right_hand);
        Cursor.lockState = CursorLockMode.Locked;
        left_hand.SetActive(true);
        this.transform.position = location;
        this.GetComponent<Animator>().SetBool("Fighting", false);
        familiar_right.transform.position = familiar_location;
        if (familiar_left != null) {
            familiar_left.transform.position = familiar2_location;
        }
        GetComponent<Player_movement>().enabled = true;
        top_down_camera.SetActive(false);
        player_camera.SetActive(true);
        this.GetComponent<Change_World>().change_combat_state();
        UI.SetActive(false);
        GameObject drop = Instantiate<GameObject>(loot);
        drop.transform.position = drop_location;
    }
}
