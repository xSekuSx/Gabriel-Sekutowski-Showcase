#pragma once
#include <irrlicht.h>
#include <string>
#include <vector>
#include <Svec.h>
#include <SekRigidBody.h>
#include <SoundAgent.h>
#include <ModelNode.h>

class GameObject
{
public:
	enum class Primitives { NoPrimitive = -1, Empty, Sphere, Cube, Plane, Cylinder, PrimitivesSize};
	enum class Tags : int { NoTag = -1, SPAWN = 0, PLAYER, NEXT_LEVEL, END };

	~GameObject();
	void deleteComponents();
	GameObject(bool temp);
	GameObject(std::string name = "Game Object", Svec3 position_vec3 = Svec3(), Svec3 rotation_vec3 = Svec3(0), Svec3 object_scale = Svec3(1.0f), Primitives primitive_shape = Primitives::Empty, Tags game_tag = Tags::NoTag,
				SekRigidBody* SekRB_ptr = nullptr, SoundAgent* Sagent_ptr = nullptr, ModelNode* Mnode_ptr = nullptr);

	int get_id();
	Svec3 get_position();
	Svec3 get_scale();
	Svec3 get_rotation();
	SekRigidBody* get_rigidbody();
	SoundAgent* get_sound_agent();
	ModelNode* get_model_node();
	std::string get_name();

	Svec3 get_forward_vector();
	Svec3 get_up_vector();
	Primitives get_primitive() { return primitive; };
	Tags get_tag() { return tag; };

	void set_rigid_body(SekRigidBody* SekRB_ptr);
	void set_sound_agent(SoundAgent* Sagent_ptr);
	void set_model_node(ModelNode* Mnode_ptr);
	void set_name(std::string new_name);
	void set_position(Svec3 pos);
	void set_rotation(Svec3 rot);
	void set_scale(Svec3 vec3);
	void set_forward(Svec3 new_forward);

	void set_primitive(Primitives new_primitive);
	void set_tag(Tags new_tag);

	static std::vector<GameObject*> GameObjects;
	static std::vector<GameObject*> DynamicGameObjects;
	static std::vector<GameObject*> SoundGameObjects;

	static void clear_vectors();
protected:
	int id;
	Tags tag;

	std::string name;
	Svec3 position;
	Svec3 rotation;
	Svec3 scale;
	Svec3 forward;
	Svec3 up;
	Primitives primitive;
	SekRigidBody* rigid_body;
	SoundAgent* sound_agent;
	ModelNode* model_node;
};