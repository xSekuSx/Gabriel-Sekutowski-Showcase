#include "GameObject.h"

std::vector<GameObject*> GameObject::GameObjects;
std::vector<GameObject*> GameObject::DynamicGameObjects;
std::vector<GameObject*> GameObject::SoundGameObjects;

GameObject::~GameObject()
{
}

void GameObject::deleteComponents()
{
	delete model_node;
	delete sound_agent;
	delete rigid_body;
}

GameObject::GameObject(bool temp)
{
}

GameObject::GameObject(std::string game_object_name, Svec3 position_vec3, Svec3 rotation_vec3, Svec3 object_scale, Primitives primitive_shape, Tags game_tag,
						SekRigidBody* SekRB_ptr, SoundAgent* Sagent_ptr, ModelNode* Mnode_ptr)
{
	id = GameObjects.size();
	name = game_object_name;
	position = position_vec3;
	rotation = rotation_vec3;
	primitive = primitive_shape;
	scale = object_scale;
	rigid_body = SekRB_ptr;
	sound_agent = Sagent_ptr;
	model_node = Mnode_ptr;
	
	tag = game_tag;
	forward = Svec3(0, 0, 1);
	up = Svec3(0, 1, 0);

	GameObjects.push_back(this);
	if (rigid_body) {
		DynamicGameObjects.push_back(this);
	}
	if (sound_agent) {
		SoundGameObjects.push_back(this);
	}
}

void GameObject::set_rigid_body(SekRigidBody* SekRB_ptr)
{
	if (rigid_body) {
		for (int i = 0; i < DynamicGameObjects.size(); ++i) {
			if (DynamicGameObjects[i] == this) {
				rigid_body->~SekRigidBody();
				DynamicGameObjects.erase(DynamicGameObjects.begin() + i);
			}
		}
	}
	rigid_body = SekRB_ptr;
	if (rigid_body) {
		DynamicGameObjects.push_back(this);
	}
}

void GameObject::set_sound_agent(SoundAgent* Sagent_ptr)
{
	if (sound_agent) {
		for (int i = 0; i < SoundGameObjects.size(); ++i) {
			if (SoundGameObjects[i] == this) {
				sound_agent->~SoundAgent();
				SoundGameObjects.erase(SoundGameObjects.begin() + i);
			}
		}
	}
	sound_agent = Sagent_ptr;
	if (sound_agent) {
		SoundGameObjects.push_back(this);
	}
}

void GameObject::set_model_node(ModelNode* Mnode_ptr)
{
	if (model_node) {
		model_node->~ModelNode();
	}
	model_node = Mnode_ptr;
}

void GameObject::set_name(std::string new_name)
{
	name = new_name;
}

void GameObject::set_position(Svec3 pos)
{
	position = pos;
}

void GameObject::set_rotation(Svec3 rot)
{
	rotation = rot;
}

void GameObject::set_scale(Svec3 vec3)
{
	scale = vec3;
	if (rigid_body) {
		rigid_body->set_GO_scale(scale);
	}
	if (model_node) {
		model_node->set_GO_scale(scale);
	}
}

int GameObject::get_id()
{
	return id;
}

Svec3 GameObject::get_position()
{
	return position;
}

Svec3 GameObject::get_scale()
{
	return scale;
}

Svec3 GameObject::get_rotation()
{
	return rotation;
}

SekRigidBody* GameObject::get_rigidbody()
{
	return rigid_body;
}

SoundAgent* GameObject::get_sound_agent()
{
	return sound_agent;
}

ModelNode* GameObject::get_model_node()
{
	return model_node;
}

void GameObject::set_primitive(Primitives new_primitive)
{
	primitive = new_primitive;
}

void GameObject::set_tag(Tags new_tag)
{
	tag = new_tag;
}

void GameObject::clear_vectors()
{
	GameObjects.clear();
	DynamicGameObjects.clear();
	SoundGameObjects.clear();
}

void GameObject::set_forward(Svec3 new_forward)
{
	forward = new_forward;
}

Svec3 GameObject::get_forward_vector()
{
	return forward;
}

Svec3 GameObject::get_up_vector()
{
	return up;
}

std::string GameObject::get_name()
{
	return name;
}