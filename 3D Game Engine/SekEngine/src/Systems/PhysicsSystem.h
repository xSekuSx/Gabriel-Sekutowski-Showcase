#pragma once
#include "SubSystem.h"
#include "Svec.h"
#include "btBulletDynamicsCommon.h"
#include "SekRigidBody.h"
#include "Player.h"

class PhysicsSystem: SubSystem {
private:
	btVector3 gravity = btVector3(0, -9.81f, 0);
	bool set_all_active = false;
public:
	float movement_multiplier = 2.5f;

	btBroadphaseInterface* broadPhase;
	btCollisionConfiguration* collConfig;
	btCollisionDispatcher* dispatcher;
	btConstraintSolver* solver;
	float dt = 1.f / 60.f;
	btDynamicsWorld* world;
	PhysicsSystem();
	~PhysicsSystem();
	void Init_Subsystem();
	void Update_Subsystem();
	void Process_events();
	//deconstructor calls this, all parts of physics are destroyed
	void Destroy_physics();
	//deletes all the phyciscs components but the world, used when a new level is loaded
	void delete_world_objects();
	//call after gameObjects have been loaded, takes rigid bodies and adds them to the world
	void load_scene_physics();
	//changes the simulation state, i.e. testing the level
	void change_sim_state();
	//aplly impulse to some object
	void apply_impulse(int object_phys_id);
	//casts a ray from the camera
	void ray_cast();
	//adds a SekRigidBody to a gameobject
	void add_SekRigidBody(Event* event);
	//moves the player based on key input enum
	void move_player(Event* event);
	//creates a raycast a depending what it hits it can load a new level or end the game
	void Shoot();
};