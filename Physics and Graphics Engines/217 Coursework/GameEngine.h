#pragma once

#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#pragma comment(lib, "glew32.lib") 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <cmath>

#include "soil/SOIL.h"

#include "shader.h"
#include "vertex.h"

#include "Model.h"
#include "Skybox.h"
#include "GameObject.h"
#include "Generator.h"
#include "RigidBody3D.h"
#include "Grid.h"

class GameEngine
{
private:
	static std::vector<GameObject*> objects;
	static std::vector<RigidBody3D*> moving_objects;
	static GameObject* CameraFollow;
	static std::vector<Model*> World;
	static std::vector<Model*> Props;
	//static Grid Chunks;
	static int oldTimeSinceStart, newTimeSinceStart;
	static int window_width;
	static int window_height;
	static float waveTime;
	static bool headlights;
	static void UpdateGame();
	static void DrawScene();
	static void CleanUpEngine();
	static void SetUp();
public:
	static int view;
	void InitEngine(int argc, char** argv, const char* windowTitle, int width, int height);
	void AddModelsToWorld(std::vector<Model*> models_vector);
	void AddModelsToProps(std::vector<Model*> models_vector);
	void AddGameObject(GameObject* object, std::vector<Model*> models, bool cam);
	void StartEngine();
};

