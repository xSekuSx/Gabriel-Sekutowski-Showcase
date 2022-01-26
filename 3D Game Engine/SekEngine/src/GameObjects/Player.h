#pragma once
#include <GameObject.h>
#include <Camera.h>

class Player : public GameObject
{
public:
	~Player();
	Player(std::string name = "Player", Svec3 position_vec3 = Svec3(0), Svec3 rotation_vec3 = Svec3(0), Svec3 object_scale = Svec3(1.0f),  Primitives primitive_shape = Primitives::NoPrimitive, Tags player_tag = Tags::PLAYER,
		SekRigidBody* SekRB_ptr = nullptr, SoundAgent* Sagent = nullptr, ModelNode* Mnode_ptr = nullptr);
	static std::vector<Player*> Players;
	Svec3 get_camera_position();
	void update_camera_position();
private:
	Svec3 camera_position;
	Camera camera;
};

