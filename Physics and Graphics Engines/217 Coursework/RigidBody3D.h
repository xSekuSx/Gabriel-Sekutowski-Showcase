#pragma once

#include "GameObject.h"
#include "Collider.h"
#include <string>

struct Zone {
	float force;
	vec3 force_dir;
};

class RigidBody3D: public GameObject
{
protected:
	float inertia, mass, mu, friction, torque, brake; //mu = coeficient of friction
	glm::vec3 angular_velocity, angular_acceleration, moments, forces, linear_velocity, linear_acceleration;
	glm::mat4 inertia_mat;
	std::vector<Zone> Zones;
public:
	RigidBody3D();
	RigidBody3D(glm::vec3 pos, float m, std::string shape, float res, float ori, bool moving);
	~RigidBody3D();

	virtual void Update_model_pos();
	virtual void Calculate_forces(float);
	virtual void Update(float delta_time);
	virtual void Update_once();

	float Get_velocity_length();
	glm::vec3 Get_lin_velocity();
	glm::vec3 Get_ang_velocity();
	void Set_lin_velocity(glm::vec3);
	void Set_ang_velocity(glm::vec3);

	float Get_mass();
	float Get_inertia();
	mat4 Get_inertia_mat();

	virtual void Add_zone(float f, vec3 fd);
};

