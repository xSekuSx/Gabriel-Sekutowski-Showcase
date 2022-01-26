#include "ModelNode.h"

ModelNode::~ModelNode()
{
	if (!anim_node) {
		delete anim_node;
	}
	else {
		delete mesh_node;
	}
}

ModelNode::ModelNode(std::string m_path, std::string text_path, Svec3 pos_offset, Svec3 rot_offset, Svec3 m_scale, Svec3 gameObject_scale)
{
	model_mesh_path = m_path;
	texture_path = text_path;

	position_offset = pos_offset;
	rotational_offset = rot_offset;
	model_scale = m_scale;
	GO_scale = gameObject_scale;

	anim_node = nullptr;
	mesh_node = nullptr;
}

void ModelNode::set_anim_node(irr::scene::IAnimatedMeshSceneNode* node_ptr)
{
	anim_node = node_ptr;
}

void ModelNode::set_mesh_node(irr::scene::IMeshSceneNode* node_ptr)
{
	mesh_node = node_ptr;
}

void ModelNode::set_position_offset(Svec3 pos)
{
	position_offset = pos;
}

void ModelNode::set_rotational_offset(Svec3 rot)
{
	rotational_offset = rot;
}

void ModelNode::set_model_scale(Svec3 scale)
{
	model_scale = scale;
	Svec3 total_scale = Svec3::multiply(model_scale, GO_scale);
	if (anim_node) {
		anim_node->setScale(Svec3::Convert_to_irr(total_scale));
	}
	else if (mesh_node) {
		mesh_node->setScale(Svec3::Convert_to_irr(total_scale));
	}
}

void ModelNode::set_GO_scale(Svec3 scale)
{
	GO_scale = scale;
	Svec3 total_scale = Svec3::multiply(model_scale, GO_scale);
	if (anim_node) {
		anim_node->setScale(Svec3::Convert_to_irr(total_scale));
	}
	else if (mesh_node) {
		mesh_node->setScale(Svec3::Convert_to_irr(total_scale));
	}
}

void ModelNode::set_model_mesh_path(std::string new_mesh_path)
{
	model_mesh_path = new_mesh_path;
}

void ModelNode::set_model_texture_path(std::string new_texture_path)
{
	texture_path = new_texture_path;
}

void ModelNode::update_model(Svec3 new_pos, Svec3 new_rot)
{
	if (anim_node) {
		anim_node->setPosition(Svec3::Convert_to_irr(new_pos + position_offset));
		anim_node->setRotation(Svec3::Convert_to_irr(new_rot + rotational_offset));
	}
	else if (mesh_node) {
		mesh_node->setPosition(Svec3::Convert_to_irr(new_pos + position_offset));
		mesh_node->setRotation(Svec3::Convert_to_irr(new_rot + rotational_offset));
	}
}

void ModelNode::update_scale()
{
	if (anim_node) {
		anim_node->setScale(Svec3::Convert_to_irr(Svec3::multiply(model_scale,GO_scale)));
	}
	else if (mesh_node) {
		mesh_node->setScale(Svec3::Convert_to_irr(Svec3::multiply(model_scale, GO_scale)));
	}
}

std::string ModelNode::get_model_mesh_path()
{
	return model_mesh_path;
}

std::string ModelNode::get_texture_path()
{
	return texture_path;
}

Svec3 ModelNode::get_position_offset()
{
	return position_offset;
}

Svec3 ModelNode::get_rotational_offset()
{
	return rotational_offset;
}

Svec3 ModelNode::get_model_scale()
{
	return model_scale;
}

irr::scene::ISceneNode* ModelNode::getNode()
{
	if (anim_node) {
		return anim_node;
	}
	else if (mesh_node) {
		return mesh_node;
	}
	return nullptr;
}

irr::scene::IAnimatedMeshSceneNode* ModelNode::get_anim_node()
{
	return anim_node;
}

irr::scene::IMeshSceneNode* ModelNode::get_mesh_node()
{
	return mesh_node;
}
