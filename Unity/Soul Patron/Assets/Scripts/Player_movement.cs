using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player_movement : MonoBehaviour
{
    CharacterController controller;
    Animator animator;

    public float run_speed = 10f;
    public float walk_speed = 5f;
    public float rotation_speed = 3f;

    public Transform ground_check;
    public float ground_distance = 0.4f;
    public LayerMask ground_mask;

    Vector3 velocity;
    bool is_grounded;

    float gravity = 9.81f;

    private void Start()
    {
        animator = GetComponent<Animator>();
        controller = GetComponent<CharacterController>();
    }

    void Update()
    {
        is_grounded = Physics.CheckSphere(ground_check.position, ground_distance, ground_mask);

        if (is_grounded && velocity.y < 0)
        {
            velocity.y = -1f;
        }

        float x = Input.GetAxis("Horizontal"); //get's unitys horizontal input W/S
        float z = Input.GetAxis("Vertical"); //A D

        if (Input.GetKey(KeyCode.LeftShift)) {
            
            Vector3 dir = transform.right * x + transform.forward * z;

            controller.Move(dir * run_speed * Time.deltaTime);

            animator.SetBool("Running", true);

            if (dir == Vector3.zero) {
                animator.SetInteger("Direction", -1);
            }
            else if (Input.GetKey(KeyCode.W) && Input.GetKey(KeyCode.D)) {
                animator.SetInteger("Direction", 1);
            }
            else if (Input.GetKey(KeyCode.S) && Input.GetKey(KeyCode.D)) {
                animator.SetInteger("Direction", 3);
            }
            else if (Input.GetKey(KeyCode.S) && Input.GetKey(KeyCode.A)) {
                animator.SetInteger("Direction", 5);
            }
            else if (Input.GetKey(KeyCode.W) && Input.GetKey(KeyCode.A)) {
                animator.SetInteger("Direction", 7);
            }
            else if (Input.GetKey(KeyCode.W)) {
                animator.SetInteger("Direction", 0);
            }
            else if (Input.GetKey(KeyCode.D)) {
                animator.SetInteger("Direction", 2);
            }
            else if (Input.GetKey(KeyCode.S)) {
                animator.SetInteger("Direction", 4);
            }
            else if (Input.GetKey(KeyCode.A)) {
                animator.SetInteger("Direction", 6);
            }
        }
        else {
            Vector3 dir = transform.forward * z;

            controller.Move(dir * walk_speed * Time.deltaTime);

            animator.SetBool("Running", false);

            if (dir == Vector3.zero) {
                animator.SetInteger("Direction", -1);
            }
            else if (Input.GetKey(KeyCode.W)) {
                animator.SetInteger("Direction", 0);
            }
            else if (Input.GetKey(KeyCode.S)) {
                animator.SetInteger("Direction", 4);
            }
        }

        if (Input.GetKey(KeyCode.Q)) {
            transform.Rotate(0.0f, -rotation_speed, 0.0f);
        }
        else if (Input.GetKey(KeyCode.E)) {
            transform.Rotate(0.0f, rotation_speed, 0.0f);
        }

        velocity.y -= gravity * Time.deltaTime;

        controller.Move(velocity * Time.deltaTime);

    }
}
