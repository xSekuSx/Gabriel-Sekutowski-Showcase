#include "GameEngine.h"
using namespace std;
using namespace glm;

#include <glm/gtx/string_cast.hpp>

std::vector<GameObject*> GameEngine::objects;
std::vector<RigidBody3D*> GameEngine::moving_objects;
int GameEngine::oldTimeSinceStart, GameEngine::newTimeSinceStart;
int GameEngine::view = 1;
float GameEngine::waveTime = 0.0f;
bool GameEngine::headlights = true;
GameObject* GameEngine::CameraFollow;

std::vector<Model*> GameEngine::World;
std::vector<Model*> GameEngine::Props;
//Grid GameEngine::Chunks;//bins

static mat4 modelViewMat = mat4(1.0);;
static mat4 projMat = mat4(1.0);
static mat3 normalMat = mat3(1.0);
int GameEngine::window_width;
int GameEngine::window_height;

enum object { DEBUG_SPHERE, DEBUG_AABB, DEBUG_PLANE, DEBUG_OBB, SKYBOX, MUSTANG, DEFAULT_CUBE, GROUND, WATER, RACETRACK, BACK_WHEELS, LEFT_WHEEL, RIGHT_WHEEL, CONE, ROAD_BLOCK, VAO_SIZE }; // VAO ids.
enum buffer { DEBUG_SPHERE_VERTICES, DEBUG_AABB_VERTICES, DEBUG_PLANE_VERTICES, DEBUG_OBB_VERTICES, SKYBOX_VERTICES, MUSTANG_VERTICES, DEFAULT_CUBE_VERTICES, GROUND_VERTICES, WATER_VERTICES, RACETRACK_VERTICES, BACK_WHEELS_VERTICES, LEFT_WHEEL_VERTICES, RIGHT_WHEEL_VERTICES, CONE_VERTICES, ROAD_BLOCK_VERTICES, VBO_SIZE }; // VBO ids.

const int vao_n = VAO_SIZE;
const int vbo_n = VBO_SIZE;
const int texture_n = 7;

static unsigned int
programId,
vertexShaderId,
fragmentShaderId,
modelMatLoc,
viewMatLoc,
projMatLoc,
objectLoc,
grassTexLoc,
waterTexLoc,
raceTexLoc,
mustangTexLoc,
wheelTexLoc,
coneTexLoc,
road_blockTexLoc,
viewPosLoc,
LHeadlightCoordsLoc,
RHeadlightCoordsLoc,
LHeadlightDirLoc,
RHeadlightDirLoc,
vao[vao_n],
buffer[vbo_n],
texture[texture_n];

struct Material
{
	vec4 ambRefl;
	vec4 difRefl;
	vec4 specRefl;
	vec4 emitCols;
	float shininess;
};

struct Light
{
	vec4 ambCols;
	vec4 difCols;
	vec4 specCols;
	vec3 coords;
	vec3 direction;
	float cutOff;
	float outerCutOff;
	float constant;
	float linear;
	float quadratic;
};

static const Material Car =
{
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(0.0, 0.0, 0.0, 1.0),
	80.0f
};

static const Material Rest =
{
	vec4(0.9, 0.9, 0.9, 1.0),
	vec4(0.9, 0.9, 0.9, 1.0),
	vec4(0.9, 0.9, 0.9, 1.0),
	vec4(0.0, 0.0, 0.0, 1.0),
	50.0f
};

static Light Sun =
{
	vec4(0.52, 0.285, 0.215, 1.0),
	vec4(0.6, 0.4, 0.4, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec3(0.0, 0.0, 0.0),
	vec3(-2.0, 3.0, -2.0),
};

static Light headlight =
{
	vec4(0.8, 0.8, 0.8, 1.0),
	vec4(0.2, 0.2, 0.2, 1.0),
	vec4(0.5, 0.5, 0.5, 1.0),
	vec3(1.0, 2.0, 1.0),
	vec3(0.0, 0.0, -1.0),
	glm::cos(glm::radians(7.5f)),
	glm::cos(glm::radians(35.0f)),
	0.8f,
	0.06f,
	0.006f,
};

Model* DebugSphere = new Model("Models/debugsphere.obj", glm::vec3(1.0f), glm::vec3(0), 0.0f, DEBUG_SPHERE, DEBUG_SPHERE_VERTICES);
Model* DebugAabb = new Model("Models/aabb.obj", glm::vec3(1.0f), glm::vec3(0), 0.0f, DEBUG_AABB, DEBUG_AABB_VERTICES);
Model* DebugPlane = new Model("Models/plane.obj", glm::vec3(1.0f), glm::vec3(0), 0.0f, DEBUG_PLANE, DEBUG_PLANE_VERTICES);
Model* DebugObb = new Model("Models/obb.obj", glm::vec3(1.0f), glm::vec3(0), 0.0f, DEBUG_OBB, DEBUG_OBB_VERTICES);
Skybox skybox;

float cameraYaw = -90;
float cameraPitch;

int lastFrameMouseX = 0;
int lastFrameMouseY = 0;

vec3 new_eye = vec3(0.0, 0.0, 0.0);
vec3 new_centre = vec3(0.0, 0.0, 0.0f);

vec3 eye = vec3(0.0, 0.0, 0.0);
vec3 centre = vec3(0.0, 0.0, 0.0f);
vec3 up = vec3(0.0, 1.0, 0.0);
vec3 cameraForward = centre - eye;
vec3 cameraPosition = vec3(0.0, -10.0, 0.0);

float headlight_dist = 1.21f * 2.0f; //2D

void GameEngine::SetUp(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	// Create shader program executable.
	vertexShaderId = setShader("vertex", "vertexShader.glsl");
	fragmentShaderId = setShader("fragment", "fragmentShader.glsl");
	programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	glUseProgram(programId);

	//codes for OpenGL lighting
	glUniform4fv(glGetUniformLocation(programId, "Car.ambRefl"), 1, &Car.ambRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "Car.difRefl"), 1, &Car.difRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "Car.specRefl"), 1, &Car.specRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "Car.emitCols"), 1, &Car.emitCols[0]);
	glUniform1f(glGetUniformLocation(programId, "Car.shininess"), Car.shininess);

	glUniform4fv(glGetUniformLocation(programId, "Rest.ambRefl"), 1, &Rest.ambRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "Rest.difRefl"), 1, &Rest.difRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "Rest.specRefl"), 1, &Rest.specRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "Rest.emitCols"), 1, &Rest.emitCols[0]);
	glUniform1f(glGetUniformLocation(programId, "Rest.shininess"), Rest.shininess);

	glUniform4fv(glGetUniformLocation(programId, "Sun.ambCols"), 1, &Sun.ambCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "Sun.difCols"), 1, &Sun.difCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "Sun.specCols"), 1, &Sun.specCols[0]);
	glUniform3fv(glGetUniformLocation(programId, "Sun.direction"), 1, &Sun.direction[0]);

	glUniform4fv(glGetUniformLocation(programId, "Lheadlight.ambCols"), 1, &headlight.ambCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "Lheadlight.difCols"), 1, &headlight.difCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "Lheadlight.specCols"), 1, &headlight.specCols[0]);
	glUniform3fv(glGetUniformLocation(programId, "Lheadlight.coords"), 1, &headlight.coords[0]);
	glUniform3fv(glGetUniformLocation(programId, "Lheadlight.direction"), 1, &headlight.direction[0]);
	glUniform1fv(glGetUniformLocation(programId, "Lheadlight.cutOff"), 1, &headlight.cutOff);
	glUniform1fv(glGetUniformLocation(programId, "Lheadlight.outerCutOff"), 1, &headlight.outerCutOff);
	glUniform1fv(glGetUniformLocation(programId, "Lheadlight.constant"), 1, &headlight.constant);
	glUniform1fv(glGetUniformLocation(programId, "Lheadlight.linear"), 1, &headlight.linear);
	glUniform1fv(glGetUniformLocation(programId, "Lheadlight.quadratic"), 1, &headlight.quadratic);

	glUniform4fv(glGetUniformLocation(programId, "Rheadlight.ambCols"), 1, &headlight.ambCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "Rheadlight.difCols"), 1, &headlight.difCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "Rheadlight.specCols"), 1, &headlight.specCols[0]);
	glUniform3fv(glGetUniformLocation(programId, "Rheadlight.coords"), 1, &headlight.coords[0]);
	glUniform3fv(glGetUniformLocation(programId, "Rheadlight.direction"), 1, &headlight.direction[0]);
	glUniform1fv(glGetUniformLocation(programId, "Rheadlight.cutOff"), 1, &headlight.cutOff);
	glUniform1fv(glGetUniformLocation(programId, "Rheadlight.outerCutOff"), 1, &headlight.outerCutOff);
	glUniform1fv(glGetUniformLocation(programId, "Rheadlight.constant"), 1, &headlight.constant);
	glUniform1fv(glGetUniformLocation(programId, "Rheadlight.linear"), 1, &headlight.linear);
	glUniform1fv(glGetUniformLocation(programId, "Rheadlight.quadratic"), 1, &headlight.quadratic);

	glUniform1f(glGetUniformLocation(programId, "lights"), GameEngine::headlights);

	// Obtain modelview matrix uniform and object uniform locations.
	modelMatLoc = glGetUniformLocation(programId, "modelMat");   //uniform mat4 modelViewMat;
	objectLoc = glGetUniformLocation(programId, "object");  //uniform uint object;
	viewPosLoc = glGetUniformLocation(programId, "viewPos");
	viewMatLoc = glGetUniformLocation(programId, "viewMat");
	LHeadlightCoordsLoc = glGetUniformLocation(programId, "Lheadlight.coords");
	RHeadlightCoordsLoc = glGetUniformLocation(programId, "Rheadlight.coords");
	LHeadlightDirLoc = glGetUniformLocation(programId, "Lheadlight.direction");
	RHeadlightDirLoc = glGetUniformLocation(programId, "Rheadlight.direction");

	// Create VAOs and VBOs
	glGenVertexArrays(vao_n, vao); //perhaps change to object number variable

	Props.push_back(DebugSphere);
	Props.push_back(DebugPlane);
	Props.push_back(DebugAabb);
	Props.push_back(DebugObb);

	for (Model* m : World) {
		//Binding VAO and VBO
		glGenBuffers(1, &buffer[m->Getid("vbo")]); ///generate one id for VBO
		m->SetIDs(vao[m->Getid("vao")], buffer[m->Getid("vbo")], 0);
		m->Setup();
	}

	for (Model* m : Props) {
		//Binding VAO and VBO
		glGenBuffers(1, &buffer[m->Getid("vbo")]); ///generate one id for VBO
		m->SetIDs(vao[m->Getid("vao")], buffer[m->Getid("vbo")], 0);
		m->Setup();
	}

	//Binding Track VAO and VBO
	glGenBuffers(1, &buffer[SKYBOX_VERTICES]);
	skybox.InitialiseSkybox(vao[SKYBOX], buffer[SKYBOX_VERTICES]);

	// Obtain projection matrix uniform location and set value.
	projMatLoc = glGetUniformLocation(programId, "projMat");   //uniform mat4 projMat;
	projMat = glm::perspective(radians(60.0), (double)window_width/(double)window_height, 0.1, 1000.0);
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));

	// Load the images.
	std::string TexNames[] = {
		 "Textures/ground.png",
		 "Textures/water.png",
		 "Textures/racetrack.png",
		 "Textures/mustangtexture.png",
		 "Textures/wheel.png",
		 "Textures/coneTexture.png",
		 "Textures/roadblockTexture.png",
	};

	// Create texture ids.
	glGenTextures(texture_n + 1, texture);

	int width, height;
	unsigned char* data;

	skybox.InitialiseCubeMap(programId, texture[texture_n -1]);

	std::string Texs[] = {"grassTex", "waterTex", "raceTex", "mustangTex", "wheelTex", "coneTex", "road_blockTex"};

	// Bind grass image.
	for (int i = 0; i < texture_n; ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texture[i]);

		//load image data using SOIL library
		data = SOIL_load_image(TexNames[i].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		SOIL_free_image_data(data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		grassTexLoc = glGetUniformLocation(programId, Texs[i].c_str());
		glUniform1i(grassTexLoc, i); //send texture to shader
	}

	//Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GameEngine::DrawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	new_centre = CameraFollow->Get_position();

	switch (view) {
	case 0:
		cameraPosition = vec3(0.0, -5.0, 0.0);
		new_centre = vec3(0.0, 0.0, 0.0);
		break;
	case 1:
		cameraPosition = vec3(0.0);
		new_eye = CameraFollow->Get_position() - vec3(CameraFollow->Get_forward_vector().x * 5.0f, -3.0f, CameraFollow->Get_forward_vector().z * 5.0f);
		new_centre += vec3(0, 2.0f, 0);
		break;
	case 2:
		cameraPosition = vec3(0.0);
		new_eye = CameraFollow->Get_position() - vec3(CameraFollow->Get_forward_vector().x * 5.0f, -3.0f, CameraFollow->Get_forward_vector().z * 5.0f);
		new_centre += vec3(0, 2.0f, 0) + CameraFollow->Get_forward_vector() + CameraFollow->force_dir;
		break;
	case 3:
		cameraPosition = vec3(0.0, -3.0f, 0.0) - (CameraFollow->Get_forward_vector() * 3.0f);
		new_eye = CameraFollow->Get_position() - vec3(CameraFollow->Get_forward_vector().x * 1.0f, CameraFollow->Get_position().y, CameraFollow->Get_forward_vector().z * 1.0f);
		break;
	case 4:
		new_eye = vec3(0.0, 10.0, 0.0);
		break;
	}

	eye = glm::mix(eye, new_eye, 0.15f);
	centre = glm::mix(centre, new_centre, 0.115f);

	if (view != 0) {
		modelViewMat = glm::lookAt(eye, centre, up);
	}

	// Calculate and update modelview matrix.
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));


	for (Model* m : World) {
		m->updateModelMatrix(modelMatLoc, cameraPosition);
		glUniform1ui(objectLoc, m->Getid("vao"));
		m->Draw();
	}

	//Draw SkyBox
	glUniform1ui(objectLoc, SKYBOX);  //if (object == SKYBOX)
	skybox.SetViewMatrix(modelMatLoc);
	skybox.Draw(programId);

	for (int i = 0; i < objects.size(); ++i)
	{
		objects[i]->Update_model_pos();
		for (Model* m : objects[i]->Get_models()) {
			m->updateModelMatrix(modelMatLoc, cameraPosition);
			glUniform1ui(objectLoc, m->Getid("vao"));
			m->Draw();
		}
		if (GameObject::debugging) {
			if (objects[i]->Has_colliders()) {
				objects[i]->Get_collider()->Update_model_pos();
				objects[i]->Get_collider()->Update_model_orientation(objects[i]->Get_orientation());
				objects[i]->Get_collider()->Update_model_size();
				objects[i]->Get_collider()->Get_model()->updateModelMatrix(modelMatLoc, cameraPosition);
				glUniform1ui(objectLoc, objects[i]->Get_collider()->Get_model()->Getid("vao"));
				objects[i]->Get_collider()->Get_model()->Draw();
			}
		}
	}

	//std::cout << to_string(CameraFollow->Get_position()) << std::endl << to_string(CameraFollow->Get_collider()->Get_position()) << std::endl << to_string(CameraFollow->Get_collider()->Get_model()->GetPosition()) << std::endl << std::endl;

	// Water settings
	waveTime += 0.004f;
	glUniform1f(glGetUniformLocation(programId, "waveTime"), waveTime);

	float Langle = glm::radians(CameraFollow->Get_orientation() - 12);
	float Rangle = glm::radians(CameraFollow->Get_orientation() + 12);
	vec3 L = vec3(glm::sin(Langle), 0, glm::cos(Langle));
	vec3 LHeadlight = vec3(glm::sin(Langle), 0, glm::cos(Langle)) * headlight_dist + CameraFollow->Get_position();
	vec3 RHeadlight = vec3(glm::sin(Rangle), 0, glm::cos(Rangle)) * headlight_dist + CameraFollow->Get_position();

	glUniform3f(viewPosLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);
	glUniform3f(LHeadlightCoordsLoc, LHeadlight.x, LHeadlight.y, LHeadlight.z);
	glUniform3f(RHeadlightCoordsLoc, RHeadlight.x, RHeadlight.y, RHeadlight.z);
	glUniform3f(LHeadlightDirLoc, CameraFollow->Get_forward_vector().x , CameraFollow->Get_forward_vector().y, CameraFollow->Get_forward_vector().z);
	glUniform3f(RHeadlightDirLoc, CameraFollow->Get_forward_vector().x , CameraFollow->Get_forward_vector().y, CameraFollow->Get_forward_vector().z);

	glutSwapBuffers();
}

void GameEngine::UpdateGame(void)
{
	oldTimeSinceStart = newTimeSinceStart;
	newTimeSinceStart = glutGet(GLUT_ELAPSED_TIME);

	float deltaTime = (newTimeSinceStart - oldTimeSinceStart) / 1000.0f;

	for (int i = 0; i < objects.size(); ++i)
	{
		objects[i]->Update(deltaTime);
	}

	for (int i = 0; i < objects.size(); ++i) {
		for (int j = (objects.size() - 1); j > i; --j) {
			if (objects[i]->Has_colliders() && objects[j]->Has_colliders()) {
				if (objects[i]->Get_collider()->CollideCheck(objects[j]->Get_collider())) {
					if (GameObject::debugging) {
						std::cout << "Collision between: " << objects[i]->Get_collider()->type << " and " << objects[j]->Get_collider()->type << std::endl;
					}
					if (objects[i]->Is_generator() || objects[j]->Is_generator()) {
						if (objects[i]->Is_generator() && objects[j]->Is_generator()) {
						}
						else if (objects[i]->Is_generator()) {
							Generator* temp = dynamic_cast<Generator*>(objects[i]);
							dynamic_cast<RigidBody3D*>(objects[j])->Add_zone(temp->gen_force, temp->gen_force_dir);
						}
						else {
							Generator* temp = dynamic_cast<Generator*>(objects[j]);
							dynamic_cast<RigidBody3D*>(objects[i])->Add_zone(temp->gen_force, temp->gen_force_dir);
						}
						Collider::Collisions_data.pop_back();
						continue;
					}
					Collider::Collisions_data[Collider::Collisions_data.size() - 1].first = objects[i];
					Collider::Collisions_data[Collider::Collisions_data.size() - 1].second = objects[j];
				}
			}
		}
	}
	
	//for (int i = -Chunks.order; i < Chunks.order + 1; ++i) {
	//	if (i == 0) {
	//		continue;
	//	}
	//	for (int j = -Chunks.order; j < Chunks.order + 1; ++j) {
	//		if (j == 0) {
	//			continue;
	//		}
	//		Bin temp_bin = Bin();
	//		temp_bin.x = i;
	//		temp_bin.z = j;
	//		vector<GameObject*> cell = Chunks.grid[temp_bin];
	//		for (int i = 0; i < cell.size(); ++i) {
	//			for (int j = (cell.size() - 1); j > i; --j) {
	//				if (cell[i]->Has_colliders() && cell[j]->Has_colliders()) {
	//					if (cell[i]->Get_collider()->CollideCheck(cell[j]->Get_collider())) {
	//						if (GameObject::debugging) {
	//							std::cout << "Collision between: " << cell[i]->Get_collider()->type << " and " << cell[j]->Get_collider()->type << std::endl;
	//						}
	//						if (cell[i]->Is_generator() || cell[j]->Is_generator()) {
	//							if (cell[i]->Is_generator() && cell[j]->Is_generator()) {
	//							}
	//							else if (cell[i]->Is_generator()) {
	//								Generator* temp = dynamic_cast<Generator*>(cell[i]);
	//								dynamic_cast<RigidBody3D*>(cell[j])->Add_zone(temp->gen_force, temp->gen_force_dir);
	//							}
	//							else {
	//								Generator* temp = dynamic_cast<Generator*>(cell[j]);
	//								dynamic_cast<RigidBody3D*>(cell[i])->Add_zone(temp->gen_force, temp->gen_force_dir);
	//							}
	//							Collider::Collisions_data.pop_back();
	//							continue;
	//						}
	//						Collider::Collisions_data[Collider::Collisions_data.size() - 1].first = cell[i];
	//						Collider::Collisions_data[Collider::Collisions_data.size() - 1].second = cell[j];
	//						if (cell[i]->does_move) {
	//							moving_objects.push_back(cell[i]);
	//						}
	//						if (cell[j]->does_move) {
	//							moving_objects.push_back(cell[j]);
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//}//bins

	for (CollisionInfo col : Collider::Collisions_data) {
		col.first->Solve_Interpenetration(col.second, col.penetration_depth, col.collision_normal);
		col.first->Solve_Impulse(col.second, col.collision_normal, col.collision_point);
	}
	Collider::Collisions_data.clear();

	//for (RigidBody3D* obj : moving_objects) {
	//	if (obj->Get_lin_velocity() == vec3(0.0f)) {
	//		remove(moving_objects.begin(), moving_objects.end(), obj);
	//	}
	//	Chunks.Check(obj);
	//}//bins

	float cos_val = glm::cos(newTimeSinceStart / 10000.0f);
	vec4 day_time = vec4(0.5f - cos_val * 0.3, 0.3f - cos_val * 0.2, 0.2f - cos_val * 0.1, 1.0f);

	glUniform4fv(glGetUniformLocation(programId, "Sun.ambCols"), 1, value_ptr(day_time));

	glutPostRedisplay();
}

void UpdateCamera()
{
	if (cameraPitch < -89)
	{
		cameraPitch = -89;
	}
	if (cameraPitch > 89)
	{
		cameraPitch = 89;
	}
	eye.x = glm::cos(glm::radians(cameraPitch)) * -glm::cos(glm::radians(cameraYaw));
	eye.y = glm::sin(glm::radians(cameraPitch));
	eye.z = glm::cos(glm::radians(cameraPitch)) * glm::sin(glm::radians(cameraYaw));

	eye = glm::normalize(eye);

	modelViewMat = glm::lookAt(eye, glm::vec3(0), up);
}

void Rotate_camera(int deltaX, int deltaY) {
	if (deltaX != 0) {
		cameraYaw += deltaX < 0 ? 1.0f : -1.0f;
	}
	if (deltaY != 0) {
		cameraPitch += deltaY < 0 ? 1.0f : -1.0f;
	}
	UpdateCamera();
}

void MouseMovementFunction(int x, int y) {
	if (GameEngine::view == 0) {
		int deltaX = x - lastFrameMouseX;
		int deltaY = y - lastFrameMouseY;

		Rotate_camera(deltaX, deltaY);

		lastFrameMouseX = x;
		lastFrameMouseY = y;
	}
}

void GameEngine::InitEngine(int argc, char** argv, const char* windowTitle, int width, int height)
{
	glutInit(&argc, argv);

	window_width = width;
	window_height = height;

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(width, height); //changed to include the parameters
	glutInitWindowPosition(350, 50);
	glutCreateWindow(windowTitle); //changed to include the parameters

	glutReshapeFunc([](int w, int h) {
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		projMat = glm::perspective(radians(60.0), (double)window_width / (double)window_height, 0.1, 1000.0);
		glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));
		glMatrixMode(GL_MODELVIEW);
		}
	); //changed to include the GameEngine function

	glutDisplayFunc(DrawScene); //changed to include the GameEngine function
	glutIdleFunc(UpdateGame); //changed to include the GameEngine function

	glutKeyboardFunc([](unsigned char key, int x, int y)
		{
			GameObject::keys[key] = true;
			//If we press escape, quit
			if (key == 27)
			{
				CleanUpEngine(); //changed to include the GameEngine function - this function will have to be static!
				exit(0);
			}
			if (key == 'f' || key == 'F') {
				++GameEngine::view;
				if (GameEngine::view > 4) {
					GameEngine::view = 0;
				}
			}
			if (key == 'r' || key == 'R') {
				GameObject::integrator++;
				if (GameObject::integrator > 1) {
					GameObject::integrator = 0;
				}
				std::cout << "Integrator: " << GameObject::integrator << std::endl;
			}
			if (key == 'e' || key == 'E') {
				GameEngine::headlights = !GameEngine::headlights;
				glUniform1f(glGetUniformLocation(programId, "lights"), GameEngine::headlights);
			}
		}
	);

	glutKeyboardUpFunc([](unsigned char key, int x, int y) {
		GameObject::keys[key] = false;
		});

	glutSpecialFunc([](int key, int x, int y) {
		GameObject::special_keys[key] = false;
		});

	glutSpecialUpFunc([](int key, int x, int y) {
		GameObject::special_keys[key] = true;
		if (key == 106) { //home button
			GameObject::debugging = !GameObject::debugging;
		}
	});

	glutPassiveMotionFunc(MouseMovementFunction);

	glewExperimental = GL_TRUE;
	glewInit();

	SetUp();
	UpdateCamera();
}

void GameEngine::AddModelsToWorld(std::vector<Model*> models_vector)
{
	World = models_vector;
}

void GameEngine::AddModelsToProps(std::vector<Model*> models_vector)
{
	Props = models_vector;
}

void GameEngine::AddGameObject(GameObject* object, std::vector<Model*> models, bool cam)
{
	if (cam) {
		CameraFollow = object;
	}
	object->AddModel(models);

	Model* temp_debug;
	if (object->Has_colliders()) {
		if (object->Get_collider()->type == "sphere") {
			temp_debug = DebugSphere;
		}
		else if (object->Get_collider()->type == "aabb") {
			temp_debug = DebugAabb;
		}
		else if (object->Get_collider()->type == "plane") {
			temp_debug = DebugPlane;
		}
		else {
			temp_debug = DebugObb;
		}
		object->Get_collider()->Attatch_model(temp_debug);
	}
	objects.push_back(object);
}

void GameEngine::StartEngine(void)
{
	std::cout << "Press Home key to enter Debug Mode\n" << "Press e to switch the lights on and off.\n" << "Press f to switch between the camreas\n" << "Press r to switch the integrator.\n" << "Press escape to exit the game.\n";
	//Chunks = Grid(100, 1);
	for (GameObject* object : objects) {
		object->Update_once();
		//if (object->Has_colliders()) {
		//	Chunks.Add_object(object);
		//}//bins
	}
	glutMainLoop();
}

void GameEngine::CleanUpEngine(void)
{
	for (int i = 0; i < objects.size(); ++i)
	{
		delete objects[i];
	}
}