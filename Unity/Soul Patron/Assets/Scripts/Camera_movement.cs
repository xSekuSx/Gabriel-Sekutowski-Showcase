using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Camera_movement : MonoBehaviour
{
    public Transform player_transform;
    public float mouse_sensitivity = 100f;
    Vector3 camera_offset;
    float minimum_height =  0f;


    // Start is called before the first frame update
    void Start()
    {
        Cursor.lockState = CursorLockMode.Locked;
        camera_offset = transform.position - player_transform.position;
    }

    // Update is called once per frame
    void Update()
    {
        float mouse_x = Input.GetAxis("Mouse X") * mouse_sensitivity * Time.deltaTime;
        float mouse_y = Input.GetAxis("Mouse Y") * mouse_sensitivity * Time.deltaTime;

        Quaternion turn_angle_x = Quaternion.AngleAxis(mouse_x, Vector3.up);
        Quaternion turn_angle_y = Quaternion.AngleAxis(mouse_y, Vector3.right);
        camera_offset = turn_angle_x * turn_angle_y * camera_offset;

        Vector3 new_position = player_transform.position + camera_offset;

        if(new_position.y < (player_transform.position.y + minimum_height)) {
            new_position.y = player_transform.position.y + minimum_height;
        }

        transform.position = Vector3.Slerp(transform.position, new_position, (mouse_sensitivity /500));
        transform.LookAt(player_transform.position);
    }
}
