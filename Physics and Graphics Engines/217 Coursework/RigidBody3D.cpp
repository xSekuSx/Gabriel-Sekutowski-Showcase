#include "RigidBody3D.h"

RigidBody3D::RigidBody3D()
{
	mass = 0.0f;
	position = vec3(0.0f);
	orientation = 0;
	mu = 0.9f;
	friction = mass * mu;
	torque = 0.0f;
	brake = 0.0f;
	restitution = 0.0f;
	brake = 0.0f;
	does_move = true;
	generator = false;
}

RigidBody3D::RigidBody3D(glm::vec3 pos, float m, std::string shape, float res, float ori, bool moving)
{
	mass = m;
	position = pos;
	orientation = ori;
	mu = 0.9f;
	friction = mass * mu * 9.81f;
	torque = 0.0f;
	brake = 0.0f;
	restitution = res;

	force_dir = glm::vec3(0.0f);
	angular_velocity = glm::vec3(0.0f);
	angular_acceleration = glm::vec3(0.0f);
	moments = glm::vec3(0.0f);

	linear_acceleration = glm::vec3(0.0f);
	linear_velocity = glm::vec3(0.0f);
	forces = glm::vec3(0.0f);
	forward = glm::vec3(1.0f, 0.0f, 0.0f);

	if (shape == "sphere") {
		//inertia = (1 / 12) * m * (length * length + width * width);
		inertia = 2.0f / 5.0f * m * 2.0f * 2.0f; // 2.0f should be r - radius
		inertia_mat = glm::mat4(1.0f) * inertia;
	}

	does_move = moving;
	generator = false;
}

RigidBody3D::~RigidBody3D()
{
}

void RigidBody3D::Update_model_pos()
{
	for (Model* m : models) {
		m->SetPosition(position);
		m->SetAngle(glm::radians(orientation));
	}
}

void RigidBody3D::Calculate_forces(float delta_time)
{
	forces = glm::vec3(0, 0, 0);
	moments = glm::vec3(0, 0, 0);
	torque = 0.0f;
	brake = 1.0f;

	for (Zone z : Zones) {
		forces += z.force * z.force_dir;
	}
	Zones.clear();

	float magnitude = glm::length(linear_velocity);

	angular_acceleration = moments / inertia;
	angular_velocity += angular_acceleration * delta_time;
	angular_velocity *= pow(0.1, delta_time);
	orientation += glm::radians(angular_velocity.y);

	forward.x = glm::sin(glm::radians(orientation));
	forward.z = glm::cos(glm::radians(orientation));
	forward = glm::normalize(forward);

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

void RigidBody3D::Update(float delta_time)
{
	Calculate_forces(delta_time);

	if (does_move) {
		if (colliders.size() > 0) {
			for (Collider* c : colliders) {
				c->Adjust_position(position);
				if (c->type == "obb") {
					c->Adjust_orientation(orientation);
				}
			}
		}
	}
}

void RigidBody3D::Update_once()
{
	if (colliders.size() > 0) {
		for (Collider* c : colliders) {
			c->Adjust_position(position);
			c->Adjust_orientation(orientation);
			c->Update_model_orientation(orientation);
		}
	}
}

float RigidBody3D::Get_velocity_length()
{
	return glm::length(linear_velocity);
}

glm::vec3 RigidBody3D::Get_lin_velocity()
{
	return linear_velocity;
}

glm::vec3 RigidBody3D::Get_ang_velocity()
{
	return angular_velocity;
}

void RigidBody3D::Set_lin_velocity(glm::vec3 new_vel)
{
	this->linear_velocity = new_vel;
}

void RigidBody3D::Set_ang_velocity(glm::vec3 new_vel)
{
	this->angular_velocity = new_vel;
}

float RigidBody3D::Get_mass()
{
	return mass;
}

float RigidBody3D::Get_inertia()
{
	return inertia;
}

mat4 RigidBody3D::Get_inertia_mat()
{
	return inertia_mat;
}

void RigidBody3D::Add_zone(float f, vec3 fd)
{
	Zone z;
	z.force = f;
	z.force_dir = fd;
	Zones.push_back(z);
}
