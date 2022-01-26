#include "GameObject.h"
#include "RigidBody3D.h"
#include "Generator.h"

#include <iostream>

std::map<char, bool> GameObject::keys;
std::map<int, bool> GameObject::special_keys;
bool GameObject::debugging = false;
int GameObject::integrator = 0;


GameObject::GameObject()
{
	does_move = false;
	orientation = 0.0f;
	restitution = 0.0f;
	generator = false;
}

GameObject::GameObject(glm::vec3 pos)
{
	position = pos;
	forward = vec3(0.0f, 0.0f, 1.0f);
	restitution = 0.5f;
	generator = false;
	orientation = 0.0f;
	restitution = 0.0f;
	does_move = false;
}

GameObject::~GameObject()
{
}

glm::vec3 GameObject::Get_position()
{
	return position;
}

void GameObject::Update_model_pos()
{
}

void GameObject::Update(float deltaTime)
{
}

void GameObject::Calculate_forces(float delta_time)
{
}

void GameObject::AddCollider(Collider* col)
{
	colliders.push_back(col);
}

void GameObject::AddModel(std::vector<Model*> m)
{
	models = m;
}

bool GameObject::Has_model()
{
	if (models.size() != 0) {
		return true;
	}
	return false;
}

std::vector<Model*> GameObject::Get_models()
{
	return models;
}

void GameObject::Solve_Interpenetration(GameObject* other, float pen_dm, vec3 normal)
{
	if (other->generator) {
		return;
	}
	if (this->does_move && other->does_move) {
		this->position -= pen_dm * normal;
		this->Get_collider()->Adjust_position(this->Get_position());
		other->position += pen_dm * normal;
		other->Get_collider()->Adjust_position(other->Get_position());
	}
	else {
		if (this->does_move) {
			this->position -= pen_dm * 2 * normal;
			this->Get_collider()->Adjust_position(this->Get_position());
		}
		else {
			other->position += pen_dm * 2 * normal;
			other->Get_collider()->Adjust_position(other->Get_position());
		}
	}
}

void GameObject::Solve_Impulse(GameObject* other, vec3 normal, vec3 point)
{
	RigidBody3D* this_temp = dynamic_cast<RigidBody3D*>(this);
	RigidBody3D* other_temp = dynamic_cast<RigidBody3D*>(other);

	vec3 impulse = vec3((-glm::dot((this_temp->Get_lin_velocity() - other_temp->Get_lin_velocity()), normal) * (1 + (this_temp->restitution + other_temp->restitution) / 2.0f)) /
		((1 / this_temp->Get_mass() + 1 / other_temp->Get_mass())
		+ glm::dot(glm::cross(normal, glm::inverse(mat3(this_temp->Get_inertia_mat())) * glm::cross(point - this_temp->Get_position(), normal)), (point - this_temp->Get_position()))
		+ glm::dot(glm::cross(normal, glm::inverse(mat3(other_temp->Get_inertia_mat())) * glm::cross(point - other_temp->Get_position(), normal)), (point - other_temp->Get_position()))));
	//std::cout << impulse.x << " " << impulse.y << " " << impulse.z << " " << std::endl;

	if (this->does_move) {
		this_temp->Set_lin_velocity(this_temp->Get_lin_velocity() + (impulse * normal / this_temp->Get_mass()));
	}
	if (other->does_move) {
		other_temp->Set_lin_velocity(other_temp->Get_lin_velocity() - (impulse * normal / other_temp->Get_mass()));
	}

	if (this->does_move) {
		//this_temp->Set_ang_velocity(this_temp->Get_ang_velocity() + glm::inverse(mat3(this_temp->Get_inertia_mat())) * (glm::cross(point, impulse * normal)));
	}
	if (other->does_move) {
		other_temp->Set_ang_velocity(other_temp->Get_ang_velocity() - glm::inverse(mat3(other_temp->Get_inertia_mat())) * (glm::cross(point, impulse * normal)));
	}
}

Collider* GameObject::Get_collider()
{
	return colliders[0];
}

bool GameObject::Has_colliders()
{
	if (colliders.size() != 0) {
		return true;
	}
	return false;
}

vec3 GameObject::Get_forward_vector()
{
	return forward;
}

float GameObject::Get_orientation()
{//orientation is in degrees
	return orientation;
}

bool GameObject::Does_move()
{
	return does_move;
}

bool GameObject::Is_generator()
{
	return generator;
}

float GameObject::Get_ori_offset()
{
	return ori_offset;
}

float GameObject::Get_velocity_length()
{
	return 0.0f;
}
