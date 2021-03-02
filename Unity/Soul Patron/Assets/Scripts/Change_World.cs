using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Change_World : MonoBehaviour {
    public Material overworld_skybox;
    public Material underworld_skybox;
    public Light overworld_light;
    public Light underworld_light;
    public GameObject enemies;

    bool underworld = false;
    float cooldown = 0f;
    bool not_in_comabat = true;


    public void change_combat_state(){
        not_in_comabat = !not_in_comabat;
    }

    void Update() {
        if (cooldown < 0 && not_in_comabat) {
            if (Input.GetKeyDown(KeyCode.Tab)) {
                cooldown = 5f;
                if (RenderSettings.skybox == overworld_skybox) {
                    Debug.Log("kanapka");
                    RenderSettings.skybox = underworld_skybox;
                    overworld_light.gameObject.SetActive(false);
                    underworld_light.gameObject.SetActive(true);
                }
                else {
                    RenderSettings.skybox = overworld_skybox;
                    underworld_light.gameObject.SetActive(false);
                    overworld_light.gameObject.SetActive(true);
                }
                underworld = !underworld;
                enemies.SetActive(underworld);
            }
        }
        else {
            cooldown = cooldown - Time.deltaTime;
        }
    }
}
