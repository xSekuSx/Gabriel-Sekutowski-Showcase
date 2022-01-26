#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#pragma comment(lib, "glew32.lib") 

#include "GameEngine.h"

#include "Particle.h"
#include "Player.h"
#include "RigidBody3D.h"
#include "SphereCollider.h"
#include "AabbCollider.h"
#include "PlaneCollider.h"
#include "ObbCollider.h"
#include "Player.h"
#include "Generator.h"

#include "soil/SOIL.h"

#include "Model.h"

//enum object { DEBUG_SPHERE, DEBUG_AABB, DEBUG_PLANE, DEBUG_OBB, MUSTANG, DEFAULT_CUBE, GROUND, WATER, RACETRACK, SKYBOX, CONE, ROAD_BLOCK, BACK_WHEELS, LEFT_WHEEL, RIGHT_WHEEL, VAO_SIZE }; // VAO ids.
//enum buffer { DEBUG_SPHERE_VERTICES, DEBUG_AABB_VERTICES, DEBUG_PLANE_VERTICES, DEBUG_OBB_VERTICES, MUSTANG_VERTICES, DEFAULT_CUBE_VERTICES, GROUND_VERTICES, WATER_VERTICES, RACETRACK_VERTICES, SKYBOX_VERTICES, CONE_VERTICES, ROAD_BLOCK_VERTICES, BACK_WHEELS_VERTICES, LEFT_WHEEL_VERTICES, RIGHT_WHEEL_VERTICES, VBO_SIZE }; // VBO ids.

enum object { DEBUG_SPHERE, DEBUG_AABB, DEBUG_PLANE, DEBUG_OBB, SKYBOX, MUSTANG, DEFAULT_CUBE, GROUND, WATER, RACETRACK, BACK_WHEELS, LEFT_WHEEL, RIGHT_WHEEL, CONE, ROAD_BLOCK, VAO_SIZE }; // VAO ids.
enum buffer { DEBUG_SPHERE_VERTICES, DEBUG_AABB_VERTICES, DEBUG_PLANE_VERTICES, DEBUG_OBB_VERTICES, SKYBOX_VERTICES, MUSTANG_VERTICES, DEFAULT_CUBE_VERTICES, GROUND_VERTICES, WATER_VERTICES, RACETRACK_VERTICES, BACK_WHEELS_VERTICES, LEFT_WHEEL_VERTICES, RIGHT_WHEEL_VERTICES, CONE_VERTICES, ROAD_BLOCK_VERTICES, VBO_SIZE }; // VBO ids.

Model* Mustang = new Model("Models/mustnag6.obj", glm::vec3(1.0f), glm::vec3(0), -90.0f, MUSTANG, MUSTANG_VERTICES);
Model* Default_cube = new Model("Models/defaultcube.obj", glm::vec3(1.0f), glm::vec3(0), 0.0f, DEFAULT_CUBE, DEFAULT_CUBE_VERTICES);
Model* Ground = new Model("Models/ground.obj", glm::vec3(10.0f), glm::vec3(0), 0.0f, GROUND, GROUND_VERTICES);
Model* Water = new Model("Models/pond.obj", glm::vec3(10.0f), glm::vec3(0), 0.0f, WATER, WATER_VERTICES);
Model* Racetrack = new Model("Models/racetrack.obj", glm::vec3(10.0f), glm::vec3(0), 0.0f, RACETRACK, RACETRACK_VERTICES);
Model* Back_wheels = new Model("Models/back_wheels.obj", glm::vec3(1.0f), glm::vec3(0, 0, 0), -90.0f, BACK_WHEELS, BACK_WHEELS_VERTICES);
Model* Left_wheel = new Model("Models/front_wheel.obj", glm::vec3(1.0f), glm::vec3(1.427, 0.243303, 0.57273), -90.0f, LEFT_WHEEL, LEFT_WHEEL_VERTICES);
Model* Right_wheel = new Model("Models/front_wheel.obj", glm::vec3(1.0f), glm::vec3(1.427, 0.243303, -0.57273), -90.0f, RIGHT_WHEEL, RIGHT_WHEEL_VERTICES);
Model* Cone = new Model("Models/cone.obj", glm::vec3(1.0f), glm::vec3(0), 0.0f, CONE, CONE_VERTICES);
Model* Road_block = new Model("Models/roadblock.obj", glm::vec3(1.0f), glm::vec3(0), 0.0f, ROAD_BLOCK, ROAD_BLOCK_VERTICES);

std::vector<Model*> World_models;
std::vector<Model*> Props_models;

GameObject* player = new Player(glm::vec3(-30, 0, 69), 1000.0f, "sphere", 0.5f, -90.0f);
GameObject* pond = new RigidBody3D(glm::vec3(0, 0, 0), 5000.0f, "sphere", 0.5f, 0.0f, false);

GameObject* wind_gen = new Generator(glm::vec3(50, 0.25f, 90), 40.0f, glm::vec3(1.0f, 0.0f, 0.0f));
GameObject* wind_gen2 = new Generator(glm::vec3(50, 0.25f, 90), 40.0f, glm::vec3(1.0f, 0.0f, 0.0f));
Collider* gen_col = new AabbCollider(50, 1, 20);

GameObject* rigidbody2 = new RigidBody3D(glm::vec3(0, 0, 30), 40.0f, "sphere", 0.5f, 0.0f, true);
Collider* sc = new SphereCollider(45.0f);
Collider* sc2 = new SphereCollider(1.0f);

Collider* aabb = new AabbCollider(1, 2, 1);
Collider* aabb2 = new AabbCollider(1, 2, 1);
Collider* aabb3 = new AabbCollider(1, 2, 1);

Collider* obb = new ObbCollider(glm::vec3(-1.5f, 0, 0), glm::vec3(0, 0.8f, 0), glm::vec3(0, 0, -0.7f));
Collider* obb2 = new ObbCollider(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1));

GameObject* go_roadBlock1 = new RigidBody3D(glm::vec3(-76, 0, -108), 2000.0f, "sphere", 0.0f, 105.0f, false);
GameObject* go_roadBlock2 = new RigidBody3D(glm::vec3(-85, 0, -103), 2000.0f, "sphere", 0.0f, -45.0f, false);
GameObject* go_roadBlock3 = new RigidBody3D(glm::vec3(-89, 0, -95), 2000.0f, "sphere", 0.0f, 0.0f, false);
GameObject* go_roadBlock4 = new RigidBody3D(glm::vec3(-27, 0, 125), 2000.0f, "sphere", 0.0f, 45.0f, false);
GameObject* go_roadBlock5 = new RigidBody3D(glm::vec3(-31, 0, 118), 2000.0f, "sphere", 0.0f, 0.0f, false);
GameObject* go_roadBlock6 = new RigidBody3D(glm::vec3(-31, 0, 112), 2000.0f, "sphere", 0.0f, 0.0f, false);

GameObject* go_cone1 = new RigidBody3D(glm::vec3(-6.5, 0, 108), 1.0f, "sphere", 0.75f, 0.0f, true);
GameObject* go_cone2 = new RigidBody3D(glm::vec3(88, 0, -85), 1.0f, "sphere", 0.75f, 0.0f, true);
GameObject* go_cone3 = new RigidBody3D(glm::vec3(91, 0, -82), 1.0f, "sphere", 0.75f, 0.0f, true);
GameObject* go_cone4 = new RigidBody3D(glm::vec3(88, 0, -78), 1.0f, "sphere", 0.75f, 0.0f, true);
GameObject* go_cone5 = new RigidBody3D(glm::vec3(84, 0, 47), 1.0f, "sphere", 0.75f, 0.0f, true);
GameObject* go_cone6 = new RigidBody3D(glm::vec3(87, 0, 49), 1.0f, "sphere", 0.75f, 0.0f, true);
GameObject* go_cone7 = new RigidBody3D(glm::vec3(85, 0, 52), 1.0f, "sphere", 0.75f, 0.0f, true);

Collider* col_roadBlock1 = new AabbCollider(2, 1, 0.3);
Collider* col_roadBlock2 = new ObbCollider(glm::vec3(2.0f, 0, 0), glm::vec3(0, 1.0f, 0), glm::vec3(0, 0, 0.3f));
Collider* col_roadBlock3 = new AabbCollider(0.3, 1, 2);
Collider* col_roadBlock4 = new ObbCollider(glm::vec3(2.0f, 0, 0), glm::vec3(0, 1.0f, 0), glm::vec3(0, 0, 0.3f));
Collider* col_roadBlock5 = new AabbCollider(0.3, 1, 2);
Collider* col_roadBlock6 = new AabbCollider(0.3, 1, 2);
Collider* col_cone1 = new AabbCollider(0.15f, 1.0f, 0.15f);
Collider* col_cone2 = new AabbCollider(0.15f, 1.0f, 0.15f);
Collider* col_cone3 = new AabbCollider(0.15f, 1.0f, 0.15f);
Collider* col_cone4 = new AabbCollider(0.15f, 1.0f, 0.15f);
Collider* col_cone5 = new AabbCollider(0.15f, 1.0f, 0.15f);
Collider* col_cone6 = new AabbCollider(0.15f, 1.0f, 0.15f);
Collider* col_cone7 = new AabbCollider(0.15f, 1.0f, 0.15f);

GameObject* ground = new RigidBody3D(glm::vec3(0, 0, 0), 500.0f, "sphere", 0.5f, 0.0f, false);
GameObject* wall1 = new RigidBody3D(glm::vec3(-155, 0, 0), 500.0f, "sphere", 0.5f, 0.0f, false);
GameObject* wall2 = new RigidBody3D(glm::vec3(135, 0, 0), 500.0f, "sphere", 0.5f, 0.0f, false);
GameObject* wall3 = new RigidBody3D(glm::vec3(0, 0, -135), 500.0f, "sphere", 0.5f, 0.0f, false);
GameObject* wall4 = new RigidBody3D(glm::vec3(0, 0, 135), 500.0f, "sphere", 0.5f, 0.0f, false);

Collider* plane_ground = new PlaneCollider(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 0.0f);
Collider* plane_wall1 = new PlaneCollider(glm::vec3(-155, 0, 0), glm::vec3(1, 0, 0), 90.0f);
Collider* plane_wall2 = new PlaneCollider(glm::vec3(135, 0, 0), glm::vec3(-1, 0, 0), 90.0f);
Collider* plane_wall3 = new PlaneCollider(glm::vec3(0, 0, -135), glm::vec3(0, 0, 1), 90.0f);
Collider* plane_wall4 = new PlaneCollider(glm::vec3(0, 0, 135), glm::vec3(0, 0, -1), 90.0f);

GameEngine engine;

// Main routine.
int main(int argc, char** argv)
{
	//adding models to the game

	World_models.push_back(Ground);
	World_models.push_back(Racetrack);
	World_models.push_back(Water);
	engine.AddModelsToWorld(World_models);
	Props_models.push_back(Mustang);
	Props_models.push_back(Default_cube);
	Props_models.push_back(Back_wheels);
	Props_models.push_back(Right_wheel);
	Props_models.push_back(Left_wheel);
	Props_models.push_back(Cone);
	Props_models.push_back(Road_block);
	engine.AddModelsToProps(Props_models);

	//attatching colliders to gameobjects


	player->AddCollider(obb);
	wind_gen2->AddCollider(aabb3);
	wind_gen->AddCollider(gen_col);
	pond->AddCollider(sc);
	ground->AddCollider(plane_ground);
	wall1->AddCollider(plane_wall1);
	wall2->AddCollider(plane_wall2);
	wall3->AddCollider(plane_wall3);
	wall4->AddCollider(plane_wall4);
	go_roadBlock1->AddCollider(col_roadBlock1);
	go_roadBlock2->AddCollider(col_roadBlock2);
	go_roadBlock3->AddCollider(col_roadBlock3);
	go_roadBlock4->AddCollider(col_roadBlock4);
	go_roadBlock5->AddCollider(col_roadBlock5);
	go_roadBlock6->AddCollider(col_roadBlock6);
	go_cone1->AddCollider(col_cone1);
	go_cone2->AddCollider(col_cone2);
	go_cone3->AddCollider(col_cone3);
	go_cone4->AddCollider(col_cone4);
	go_cone5->AddCollider(col_cone5);
	go_cone6->AddCollider(col_cone6);
	go_cone7->AddCollider(col_cone7);


	//adding gameobjects to the game

	engine.AddGameObject(player, std::vector<Model*>{Mustang, Back_wheels, Right_wheel, Left_wheel}, true); //for the player, wheels must be added last
	engine.AddGameObject(pond, std::vector<Model*>{}, false);
	engine.AddGameObject(wind_gen, std::vector<Model*>{}, false);
	engine.AddGameObject(go_roadBlock1, std::vector<Model*>{Road_block}, false);
	engine.AddGameObject(go_roadBlock2, std::vector<Model*>{Road_block}, false); //obb
	engine.AddGameObject(go_roadBlock3, std::vector<Model*>{Road_block}, false);
	engine.AddGameObject(go_roadBlock4, std::vector<Model*>{Road_block}, false); //obb
	engine.AddGameObject(go_roadBlock5, std::vector<Model*>{Road_block}, false);
	engine.AddGameObject(go_roadBlock6, std::vector<Model*>{Road_block}, false);
	engine.AddGameObject(go_cone1, std::vector<Model*>{Cone}, false);
	engine.AddGameObject(go_cone2, std::vector<Model*>{Cone}, false);
	engine.AddGameObject(go_cone3, std::vector<Model*>{Cone}, false);
	engine.AddGameObject(go_cone4, std::vector<Model*>{Cone}, false);
	engine.AddGameObject(go_cone5, std::vector<Model*>{Cone}, false);
	engine.AddGameObject(go_cone6, std::vector<Model*>{Cone}, false);
	engine.AddGameObject(go_cone7, std::vector<Model*>{Cone}, false);

	//engine.AddGameObject(ground, std::vector<Model*>{}, false);
	engine.AddGameObject(wall1, std::vector<Model*>{}, false);
	engine.AddGameObject(wall2, std::vector<Model*>{}, false);
	engine.AddGameObject(wall3, std::vector<Model*>{}, false);
	engine.AddGameObject(wall4, std::vector<Model*>{}, false);


	//starting the engine
	engine.InitEngine(argc, argv, "Basic Race'u", 1366, 768);
	engine.StartEngine();
	return 0;
}