#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <map>
#include <vector>
#include "Collider.h"
#include "Model.h"

class RigidBody3D;

class GameObject
{
protected:
	glm::vec3 position, forward;
	std::vector<Collider*> colliders;
	std::vector<Model*> models;
	float orientation;
	float ori_offset;
	bool generator;
public:
	glm::vec3 force_dir;
	float magnitude;
	static std::map <char, bool> keys;
	static std::map <int, bool> special_keys;
	static bool debugging;
	static int integrator;
	float restitution;
	bool does_move;

	GameObject();
	GameObject(glm::vec3 pos);
	~GameObject();
	virtual glm::vec3 Get_position();
	virtual void Update_model_pos();
	virtual void Update(float delta_time) = 0;
	virtual void Update_once() = 0;
	virtual void Calculate_forces(float delta_time);
	virtual void AddCollider(Collider*);
	virtual void AddModel(std::vector<Model*>);

	virtual bool Has_model();
	virtual std::vector<Model*> Get_models();

	virtual void Solve_Interpenetration(GameObject*, float, vec3);
	virtual void Solve_Impulse(GameObject* other, vec3 normal, vec3 point);

	virtual bool Has_colliders();
	virtual Collider* Get_collider();
	virtual vec3 Get_forward_vector();
	virtual float Get_orientation();
	virtual bool Does_move();
	virtual bool Is_generator();
	virtual float Get_ori_offset();
	virtual float Get_velocity_length();
};