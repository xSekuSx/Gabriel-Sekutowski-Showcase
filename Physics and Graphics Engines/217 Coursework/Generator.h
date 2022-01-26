#pragma once

#include "GameObject.h"

class RigidBody3D;

class Generator : public GameObject
{
private:
public:
	float gen_force;
	vec3 gen_force_dir;

	Generator(vec3 pos, float f, vec3 fd);
	~Generator();
	void Solve_Interpenetration(GameObject*, float, vec3);
	void Solve_Impulse(GameObject* other, vec3 normal, vec3 point);
	void Update(float delta_time);
	void Update_once();
};

