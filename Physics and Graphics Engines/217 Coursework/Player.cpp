#include "Player.h"

Player::Player(glm::vec3 pos, float m, std::string shape, float res, float ori) : RigidBody3D(pos, m, shape, res, ori, true)
{
	force_position = glm::vec3(2.0f, 0.0f, 2.0f);
	force_value = glm::vec3(0, 0, 800.0f);
	ori_offset = ori;
	angular_velocity.y = glm::radians(ori);
	turning_angle = 0.0f;
}

Player::~Player()
{
} 

void Player::Calculate_forces(float delta_time)
{
	forces = glm::vec3(0, 0, 0);
	moments = glm::vec3(0, 0, 0);
	torque = 0.0f;
	brake = 1.0f;
	turning_angle = 0.0f;

	if (GameObject::keys['w'] || GameObject::keys['W']) {
		torque = 17000.0f;
	}
	if (GameObject::keys['s'] || GameObject::keys['S']) {
		torque = -10000.0f;
	}

	if (GameObject::keys[' ']) {
		brake = 3.0f;
	}

	magnitude = glm::length(linear_velocity);

	if (GameObject::keys['a'] || GameObject::keys['A']) {
		turning_angle = glm::radians(25.0f);
		if (torque != 0 || magnitude > 2) {
			moments += glm::cross(force_position, -force_value);
		}
	}
	if (GameObject::keys['d'] || GameObject::keys['D']) {
		turning_angle = glm::radians(-25.0f);
		if (torque != 0 || magnitude > 2) {
			moments += glm::cross(force_position, force_value);
		}
	}
	wheels[0]->Rotate(turning_angle);
	wheels[1]->Rotate(turning_angle);

	angular_acceleration = moments / inertia;
	angular_velocity += angular_acceleration * delta_time;
	orientation = glm::degrees(angular_velocity.y);

	forward.x = glm::sin(glm::radians(orientation));
	forward.z = glm::cos(glm::radians(orientation));
	forward = glm::normalize(forward);

	//std::cout << glm::to_string(position) << std::endl;

	if (magnitude != 0) {
		force_dir = glm::normalize(linear_velocity);
		forces = torque * forward - (friction * force_dir * brake);
	}
	else if (torque != 0) {
		forces = torque * forward - (friction * force_dir * brake);
	}

	if (std::abs(magnitude) < 0.001f) {
		force_dir = vec3(0.0f, 0.0f, 0.0f);
		linear_velocity = vec3(0.0f, 0.0f, 0.0f);
	}

	for (Zone z : Zones) {
		forces += z.force * z.force_dir;
	}
	Zones.clear();

	if (integrator == 0) {
		linear_acceleration = forces / mass;
		position = position + linear_velocity * delta_time;
		linear_velocity += linear_acceleration * delta_time;
	}
	else if (integrator == 1) {
		//forces += glm::vec3(0, -9.8f, 0) * mass;
		vec3 new_linear_acceleration = forces / mass;
		position = position + linear_velocity * delta_time + 1.0f / 6.0f * (4.0f * linear_acceleration - new_linear_acceleration) * delta_time * delta_time;
		linear_velocity = linear_velocity + 0.5f * (linear_acceleration + new_linear_acceleration) * delta_time;
		linear_acceleration = new_linear_acceleration;
	}
	else if (integrator == 2) {
		vec3 new_linear_acceleration = forces / mass;
	}
}

void Player::Update(float delta_time)
{
	Calculate_forces(delta_time);

	if (colliders.size() > 0) {
		for (Collider* c : colliders) {
			c->Adjust_position(position);
			if (c->type == "obb") {
				c->Adjust_orientation(orientation);
			}
		}
	}
}

void Player::AddModel(std::vector<Model*> m)
{
	models = m;
	wheels.push_back(m[m.size()-1]);
	wheels.push_back(m[m.size()-2]);
}
