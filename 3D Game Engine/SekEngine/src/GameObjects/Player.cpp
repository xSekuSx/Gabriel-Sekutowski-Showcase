#include "Player.h"

std::vector<Player*> Player::Players;

Player::~Player()
{
}

Player::Player(std::string name, Svec3 position_vec3, Svec3 rotation_vec3, Svec3 object_scale, Primitives primitive_shape, Tags player_tag ,SekRigidBody* SekRB_ptr, SoundAgent* Sagent, ModelNode* Mnode_ptr) :
	GameObject(name, position_vec3, rotation_vec3, object_scale, primitive_shape, player_tag, SekRB_ptr, Sagent, Mnode_ptr)
{
	Players.push_back(this);
}

Svec3 Player::get_camera_position()
{
	return camera_position;
}

void Player::update_camera_position()
{
	camera_position = position + Svec3(0, 8, 0);
}
