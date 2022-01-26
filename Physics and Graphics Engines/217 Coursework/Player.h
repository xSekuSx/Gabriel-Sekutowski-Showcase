#pragma once

#include "RigidBody3D.h"

class Player: public RigidBody3D
{
private:
	glm::vec3 force_position, force_value;
	std::vector<Model*> wheels;
	float turning_angle;
public:
	Player(glm::vec3 pos, float m, std::string shape, float res, float ori);
	~Player();
	void Calculate_forces(float delta_time);
	void Update(float delta_time);
	void AddModel(std::vector<Model*> m);
};