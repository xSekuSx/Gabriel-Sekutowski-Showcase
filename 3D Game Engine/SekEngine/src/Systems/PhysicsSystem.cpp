#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem()
{
}

PhysicsSystem::~PhysicsSystem()
{
	Destroy_physics();
}

void PhysicsSystem::Init_Subsystem()
{
	std::cout << "Initialising Physics" << std::endl;

	SubSystem_name = "Physics";
	running_sim = !SubSystem::level_editing;

	broadPhase = new btDbvtBroadphase();
	collConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collConfig);
	solver = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(dispatcher, broadPhase, solver, collConfig);

	world->setGravity(gravity); //setting the gravity value for the world
}

void PhysicsSystem::Update_Subsystem()
{
	if (running_sim) {
		world->stepSimulation(dt, 10);

		for (int j = 0; j < world->getNumCollisionObjects(); ++j)
		{
			btCollisionObject* obj = world->getCollisionObjectArray()[j]; //get the physics object from the world
			btRigidBody* body = btRigidBody::upcast(obj); //get its rigid body
			btTransform transform;
			body->getMotionState()->getWorldTransform(transform); //get a copy of its transform to access the variables (in this case, the origin/position)
			GameObject::DynamicGameObjects[j]->set_position(Svec3::Convert_from_bt(transform.getOrigin())); //assign rigid bodies pos to its gameoject
			if (set_all_active) {
				/*
				when the object falls and collisions is resolved they become inactive and bullet wont update them
				things could move and their position could have changed, in level editor, but object could still be inactive
				to fix this once the sim state changes, set all active becomes true and re enables all objects for one frame
				moving objects will remain active whereas stationary will deactive themselves
				*/
				body->activate(true);
			}
		}
		set_all_active = false;
	}
	Process_events();
	Profiling();
}



void PhysicsSystem::Process_events()
{
	for (int i = 0; i < EventHandler::EventQueue.size(); ++i) {
		for (int j = 0; Event::Subsystems_size; ++j) {
			if (EventHandler::EventQueue[i]->subsystems_involved[j] == Event::Physics) {
				switch (EventHandler::EventQueue[i]->type)
				{
				//case Event::EventType::Key_press:
				//	move_object(EventHandler::EventQueue[i]);
				//	break;
				case Event::EventType::Load:
					load_scene_physics();
					break;
				case Event::EventType::Move_object:
					apply_impulse(*(int*)EventHandler::EventQueue[i]->get_value());
					break;
				case Event::EventType::RayCast:
					ray_cast();
					break;
				case Event::EventType::Shoot:
					Shoot();
					break;
				case Event::EventType::Move_player:
					move_player(EventHandler::EventQueue[i]);
					break;
				case Event::EventType::ChangeSimState:
					change_sim_state();
					break;
				case Event::EventType::Add_SekRigidBody:
					add_SekRigidBody(EventHandler::EventQueue[i]);
					break;
				default:
					std::cout << "Physics function enum out of range" << std::endl;
					break;
				}
				EventHandler::EventQueue[i]->subsystems_involved[j] = Event::NoSubSystem;
			}
			else if (EventHandler::EventQueue[i]->subsystems_involved[j] > Event::Physics ||
				EventHandler::EventQueue[i]->subsystems_involved[j] == Event::End) {
				break;
			}
		}
	}
}

void PhysicsSystem::Destroy_physics()
{
	delete_world_objects();

	delete world;
	delete solver;
	delete broadPhase;
	delete dispatcher;
	delete collConfig;
}

void PhysicsSystem::load_scene_physics()
{
	for (int i = 0; i < GameObject::DynamicGameObjects.size(); ++i) {
		GameObject::DynamicGameObjects[i]->get_rigidbody()->set_physics_id(i);
		world->addRigidBody(GameObject::DynamicGameObjects[i]->get_rigidbody()->get_phy_rb());
		if (SubSystem::Debugging) {
			std::cout << GameObject::DynamicGameObjects[i]->get_name() << " " << GameObject::DynamicGameObjects[i]->get_rigidbody()->get_phy_rb()->getWorldArrayIndex() << std::endl;
		}
	}
	Player::Players[0]->set_rigid_body(new SekRigidBody());
	world->addRigidBody(Player::Players[0]->get_rigidbody()->get_phy_rb());
}

void PhysicsSystem::change_sim_state()
{
	running_sim = !running_sim;
	if (running_sim) {
		set_all_active = true;
	}
}

void PhysicsSystem::ray_cast() 
{
	btVector3 btFrom(Svec3::Convert_to_bt(Player::Players[0]->get_position()));
	btVector3 btTo(Svec3::Convert_to_bt(Player::Players[0]->get_forward_vector()) * 1000.0f);
	btCollisionWorld::ClosestRayResultCallback Raycast(btFrom, btTo);

	world->rayTest(btFrom, btTo, Raycast);

	if (Raycast.hasHit()) {
		if (SubSystem::Debugging) {
			std::cout << "Nearest Raycast" << std::endl;
			printf("Collision at: <%.2f, %.2f, %.2f>\n", Raycast.m_hitPointWorld.getX(), Raycast.m_hitPointWorld.getY(), Raycast.m_hitPointWorld.getZ());
			std::cout << Raycast.m_collisionObject->getWorldArrayIndex() << std::endl;
		}
		int id = Raycast.m_collisionObject->getWorldArrayIndex();
	}
}

void PhysicsSystem::delete_world_objects()
{
	for (int i = world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = world->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		world->removeCollisionObject(obj);
		delete obj;
	}
}

void PhysicsSystem::add_SekRigidBody(Event* event)
{
	int id = *(int*)event->get_value();
	GameObject::GameObjects[id]->set_rigid_body(new SekRigidBody());
}

void PhysicsSystem::move_player(Event* event)
{
	if (running_sim) {
		char_and_sf* key_pressed = (char_and_sf*)event->get_value();

		Svec3 pos = Player::Players[0]->get_position();
		Svec3 dir = Player::Players[0]->get_forward_vector();
		Svec3 right_vector = Player::Players[0]->get_up_vector().cross(dir);
		Svec3 distance;

		if (key_pressed->key_enum == input_key::FORWARD) {
			distance = dir * movement_multiplier;
		}
		else if (key_pressed->key_enum == input_key::BACKWARD) {
			distance = dir * -1 * movement_multiplier;
		}
		else if (key_pressed->key_enum == input_key::LEFT) {
			distance = right_vector * -1 * movement_multiplier;
		}
		else if (key_pressed->key_enum == input_key::RIGHT) {
			distance = right_vector * 1 * movement_multiplier;
		}

		distance.y = 0;

		Player::Players[0]->get_rigidbody()->get_phy_rb()->activate(true);
		Player::Players[0]->get_rigidbody()->get_phy_rb()->setLinearVelocity(Svec3::Convert_to_bt(distance));
	}
}

void PhysicsSystem::Shoot()
{
	btVector3 btFrom(Svec3::Convert_to_bt(Player::Players[0]->get_camera_position()));
	btVector3 btTo(Svec3::Convert_to_bt(Player::Players[0]->get_forward_vector()) * 1000.0f);
	btCollisionWorld::ClosestRayResultCallback Raycast(btFrom, btTo);

	world->rayTest(btFrom, btTo, Raycast);

	if (Raycast.hasHit()) {
		if (SubSystem::Debugging) {
			std::cout << "Nearest Raycast" << std::endl;
			printf("Collision at: <%.2f, %.2f, %.2f>\n", Raycast.m_hitPointWorld.getX(), Raycast.m_hitPointWorld.getY(), Raycast.m_hitPointWorld.getZ());
			std::cout << Raycast.m_collisionObject->getWorldArrayIndex() << std::endl;
		}
		int id = Raycast.m_collisionObject->getWorldArrayIndex();

		Raycast.m_collisionObject->activate(true);
		btCollisionObject* obj = world->getCollisionObjectArray()[id];

		if (SubSystem::level_editing) {
			if (GameObject::DynamicGameObjects[id]->get_tag() == GameObject::Tags::END) {
				int temp_event_sub[] = { Event::Engine };
				Event* event = new Event(Event::EventType::SendScore, temp_event_sub);
				EventHandler::add_event(event);
			}
			if (GameObject::DynamicGameObjects[id]->get_tag() == GameObject::Tags::NEXT_LEVEL) {
				int temp_event_sub[] = { Event::Engine };
				Event* event = new Event(Event::EventType::NewLevel, temp_event_sub);
				EventHandler::add_event(event);
			}
		}

		btRigidBody* body = btRigidBody::upcast(obj);
		btVector3 dir = body->getWorldTransform().getOrigin() - Svec3::Convert_to_bt(Player::Players[0]->get_position());
		body->applyImpulse(dir.normalize() * 30, Svec3::Convert_to_bt(Player::Players[0]->get_position()) - body->getWorldTransform().getOrigin());
	}
}

void PhysicsSystem::apply_impulse(int object_phys_id)
{
	GameObject::DynamicGameObjects[object_phys_id]->get_rigidbody()->get_phy_rb()->activate(true);
	//GameObject::DynamicGameObjects[0]->get_rigidbody()->get_phy_rb()->applyForce(btVector3(0.f, 0.f, -2.0f), btVector3(0.f, 0.f, 0.0f));
	GameObject::DynamicGameObjects[object_phys_id]->get_rigidbody()->get_phy_rb()->applyCentralImpulse(btVector3(0.f, 0.f, -0.3f));
	//GameObject::DynamicGameObjects[object_phys_id]->get_rigidbody()->get_phy_rb()->activate(false);

	//btRigidBody::setLinearVelocity()
}
