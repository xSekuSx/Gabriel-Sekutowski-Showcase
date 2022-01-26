#pragma once
#include <irrlicht.h>
#include "Svec.h"

class ModelNode
{
public:
	~ModelNode();

	ModelNode(std::string model_path = "", std::string texture_path = "", Svec3 pos_offset = Svec3(0), Svec3 rot_offset = Svec3(0), Svec3 m_scale = Svec3(1), Svec3 gameObject_scale = Svec3(1));

	void set_anim_node(irr::scene::IAnimatedMeshSceneNode* node_ptr);
	void set_mesh_node(irr::scene::IMeshSceneNode* node_ptr);
	void set_position_offset(Svec3 pos);
	void set_rotational_offset(Svec3 rot);
	void set_model_scale(Svec3 scale);
	void set_GO_scale(Svec3 scale);
	void set_model_mesh_path(std::string new_mesh_path);
	void set_model_texture_path(std::string new_texture_path);

	void update_model(Svec3 new_pos, Svec3 new_rot);
	void update_scale();

	std::string get_model_mesh_path();
	std::string get_texture_path();
	Svec3 get_position_offset();
	Svec3 get_rotational_offset();
	Svec3 get_model_scale();

	irr::scene::ISceneNode* getNode();
	irr::scene::IAnimatedMeshSceneNode* get_anim_node();
	irr::scene::IMeshSceneNode* get_mesh_node();

	irr::scene::IAnimatedMeshSceneNode* anim_node;
	irr::scene::IMeshSceneNode* mesh_node;
	Svec3 position_offset, rotational_offset, model_scale, GO_scale;

	std::string model_mesh_path, texture_path;
};

