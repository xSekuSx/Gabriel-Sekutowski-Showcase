#include <iostream>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SOIL.h"

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <map>
#include <random>

#pragma comment(lib, "glew32.lib") 

#pragma warning (disable : 4996)

#include "PerlinNoise.hpp"

#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

Clock::time_point CurrentTime; //current time
Clock::time_point OldTime; //time of previous frame
Clock::duration DeltaTime; //change in time between frames
float deltaTimefloat;

using namespace std;
using namespace glm;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distr(1, 10);
std::uniform_int_distribution<> posneg(1, 2);

// Size of the terrain
const int dimensions = 500;
const int MAP_SIZE = dimensions;
const int cloud_size = dimensions * 2;
const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;
//trees parameters
const int num_trees = 2; //the number of trees generated
const int trees_complexity = 270; //number of vertices per tree, a 4 is a square like mc
const int tree_height = 6; //number of subdivision per tree, branches etc. 1 is just a trunk
const float decrease = 0.7f; //the decrease of overall thickness of the tree, thickness also impacts the height
const float decrease_height = 0.93f; //decrease in height of the branches
const float branch_distance_multiplier = 2.5f; //the distance between branches
const float branch_angle = radians(20.0f); //angle at which new branches are created
float tree_edge_angle = radians(360.0f / trees_complexity); //amount of degress per step when creating each cilinder for each branch or trunk
const float tree_length_thickness_ratio = 4.0f; //ratio between thickness and length, geater this value longer trees will be
const float treethickness = 1.6f; //thickness i.e. radius of the tree
const float tree_randomness_scale = 0.6f; //the scale of the random position affetcs new bracn position
const float scale_multiplier = 100.0f; //scale of the whole terrain
//values needed to create indices
int index; 
int current_branch;
int current_tree = 0;

std::uniform_int_distribution<> tree_placement(0, dimensions * dimensions);

std::map<char, bool> keys;

float cameraSpeed = 300.0f;

bool firstMouse = true;
float Camera_yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float Camera_pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

int lastFrameMouseX = 0;
int lastFrameMouseY = 0;

// camera
glm::vec3 cameraPos = glm::vec3(dimensions / 2.f + 10, scale_multiplier / 2.f + 10, dimensions / 2.f + 10);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Obtain modelview matrix uniform location and set value.
static mat4 modelViewMat = mat4(1.0);
static mat4 projMat = mat4(1.0);
static mat4 modelMat = mat4(1.0);

struct Vertex
{
	vec4 coords;
	vec3 normals;
	vec2 texcoords;
};

struct Matrix4x4
{
	float entries[16];
};

struct Light
{
	vec3 ambCols;
	vec3 difCols;
	vec3 specCols;
	vec4 coords;
	vec3 direction;
	float cutOff;
	float outerCutOff;
	float constant;
	float linear;
	float quadratic;
};

struct light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec4 coords;
};

static const light light0 =
{
	vec3(1.0, 1.0, 1.0),
	vec3(1.0, 1.0, 1.0),
	vec3(1.0, 1.0, 1.0),
	vec4(scale_multiplier, scale_multiplier / 2.f, scale_multiplier, 1.0)
};

static Light Sun =
{
	vec3(0.82, 0.8, 0.8),
	vec3(0.6, 0.4, 0.4),
	vec3(1.0, 1.0, 1.0),
	vec4(0.0, 0.0, 0.0, 0.0),
	vec3(-2.0, 3.0, -2.0)
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

static const Material terrainMaterial =
{
	vec3(0.8, 0.8, 0.8),
	vec3(0.6, 0.6, 0.6),
	vec3(0.2, 0.2, 0.2),
	0.0f
};

static const Material waterMaterial =
{
	vec3(0.6, 0.6, 0.6),
	vec3(0.7, 0.7, 0.7),
	vec3(0.8, 0.8, 0.8),
	64.f
};

float skyboxVertices[] =
{
	-1000.0f,  1000.0f, -1000.0f,
	-1000.0f, -1000.0f, -1000.0f,
	 1000.0f, -1000.0f, -1000.0f,
	 1000.0f, -1000.0f, -1000.0f,
	 1000.0f,  1000.0f, -1000.0f,
	-1000.0f,  1000.0f, -1000.0f,

	-1000.0f, -1000.0f,  1000.0f,
	-1000.0f, -1000.0f, -1000.0f,
	-1000.0f,  1000.0f, -1000.0f,
	-1000.0f,  1000.0f, -1000.0f,
	-1000.0f,  1000.0f,  1000.0f,
	-1000.0f, -1000.0f,  1000.0f,

	 1000.0f, -1000.0f, -1000.0f,
	 1000.0f, -1000.0f,  1000.0f,
	 1000.0f,  1000.0f,  1000.0f,
	 1000.0f,  1000.0f,  1000.0f,
	 1000.0f,  1000.0f, -1000.0f,
	 1000.0f, -1000.0f, -1000.0f,

	-1000.0f, -1000.0f,  1000.0f,
	-1000.0f,  1000.0f,  1000.0f,
	 1000.0f,  1000.0f,  1000.0f,
	 1000.0f,  1000.0f,  1000.0f,
	 1000.0f, -1000.0f,  1000.0f,
	-1000.0f, -1000.0f,  1000.0f,

	-1000.0f,  1000.0f, -1000.0f,
	 1000.0f,  1000.0f, -1000.0f,
	 1000.0f,  1000.0f,  1000.0f,
	 1000.0f,  1000.0f,  1000.0f,
	-1000.0f,  1000.0f,  1000.0f,
	-1000.0f,  1000.0f, -1000.0f,

	-1000.0f, -1000.0f, -1000.0f,
	-1000.0f, -1000.0f,  1000.0f,
	 1000.0f, -1000.0f, -1000.0f,
	 1000.0f, -1000.0f, -1000.0f,
	-1000.0f, -1000.0f,  1000.0f,
	 1000.0f, -1000.0f,  1000.0f
};

static enum buffer { TERRAIN_VERTICES, WATER_VERTICES, TREE_VERTICES, SKYBOX_VERTICES, CLOUDS_VERTICES };
static enum object { TERRAIN = 0, WATER, TREE, SKYBOX, CLOUDS};
// Globals

const int numStripsRequired = MAP_SIZE - 1;
const int verticesPerStrip = 2 * MAP_SIZE;

const int cloud_numStripsRequired = cloud_size - 1;
const int cloud_verticesPerStrip = 2 * cloud_size;

const int verticesPerTreeSegment = (1 << tree_height) - 1;// ((1 << (tree_height + 1)) - 1) * 2;

unsigned int waterIndexData[numStripsRequired][verticesPerStrip];
unsigned int terrainIndexData[numStripsRequired][verticesPerStrip];
unsigned int treeIndexData[num_trees][verticesPerTreeSegment][trees_complexity][4];//4 vertcies per tirangle
unsigned int cloudIndexData[cloud_numStripsRequired][cloud_verticesPerStrip];

static Vertex terrainVertices[MAP_SIZE * MAP_SIZE] = {};
static Vertex waterVertices[MAP_SIZE * MAP_SIZE] = {};
static Vertex treeVertices[trees_complexity * (verticesPerTreeSegment + 1) * num_trees] = {};
static Vertex cloudVertices[cloud_size * cloud_size] = {};

const unsigned int texture_n = 4;
const unsigned int vao_n = 5;
const unsigned int vbo_n = 5;

static float waveTime = 0;
static float cloudTime = 0;

static unsigned int
programId,
vertexShaderId,
fragmentShaderId,
modelViewMatLoc,
normalMatLoc,
projMatLoc,
TexLoc,
vao[vao_n],
vbo[vbo_n],
objectLoc,
viewPosLoc,
texture[texture_n];

// Function to read text file, used to read shader files
char* readTextFile(const char* aTextFile)
{
	FILE* filePointer = fopen(aTextFile, "rb");
	char* content = NULL;
	long numVal = 0;

	fseek(filePointer, 0L, SEEK_END);
	numVal = ftell(filePointer);
	fseek(filePointer, 0L, SEEK_SET);
	content = (char*)malloc((numVal + 1) * sizeof(char));
	fread(content, 1, numVal, filePointer);
	content[numVal] = '\0';
	fclose(filePointer);
	return content;
}

void shaderCompileTest(GLuint shader)
{
	GLint result = GL_FALSE;
	int logLength;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(shader, logLength, NULL, &vertShaderError[0]);
	std::cout << &vertShaderError[0] << std::endl;
}

void Gen_treeVerts(int height, vec3 middle, float displacement) {
	for (int j = 0; j < trees_complexity; ++j)
	{
		treeVertices[index].coords.x = middle.x + displacement + treethickness * cos(tree_edge_angle * j) * pow(decrease, height);
		treeVertices[index].coords.y = middle.y + height * tree_length_thickness_ratio * treethickness * pow(decrease_height, height);
		treeVertices[index].coords.z = middle.z + displacement + treethickness * sin(tree_edge_angle * j) * pow(decrease, height);
		treeVertices[index].coords.w = 1.0;

		treeVertices[index].normals.x = cos(tree_edge_angle * j);
		treeVertices[index].normals.y = 0.0;
		treeVertices[index].normals.z = sin(tree_edge_angle * j);

		index++;
	}
	if (height == tree_height) {
		return;
	}

	float randomness = (distr(gen) / 10.0f) * tree_randomness_scale * (height + 1);
	(posneg(gen) == 1) ? randomness *= -1 : false;
	middle = vec3(middle.x - randomness * (tree_height - height) / 2, middle.y + randomness, middle.z + randomness * (tree_height - height) / 2);

	//left
	Gen_treeVerts(height + 1, middle, displacement - treethickness * pow(decrease, height) * branch_distance_multiplier);

	randomness = (distr(gen) / 10.0f) * tree_randomness_scale * (height + 1);
	(posneg(gen) == 1) ? randomness *= -1 : false;
	middle = vec3(middle.x + randomness * (tree_height - height) / 2, middle.y + randomness, middle.z - randomness * (tree_height - height) / 2);

	//right
	Gen_treeVerts(height + 1, middle, displacement + treethickness * pow(decrease, height) * branch_distance_multiplier);
}

void Gen_indicies(int base, int current_height) {
	if (current_height == tree_height) {
		return;
	}

	index = base;
	int secondaryIndex = trees_complexity * (current_branch + 1);
	for (int j = 0; j < trees_complexity; ++j) {
		for (int k = 0; k < 2; ++k) {
			if (index == trees_complexity * (current_branch + 1)) {
				index = trees_complexity * (current_branch);
			}
			treeIndexData[current_tree][current_branch][j][k] = index;
			++index;
		}
		--index;
		for (int k = 2; k < 4; ++k) {
			if (secondaryIndex == trees_complexity * (current_branch + 2)) {
				secondaryIndex = trees_complexity * (current_branch + 1);
			}
			treeIndexData[current_tree][current_branch][j][k] = secondaryIndex;
			++secondaryIndex;
		}
		--secondaryIndex;
	}
	current_branch++;
	//left
	Gen_indicies(secondaryIndex, current_height + 1);
	//right
	Gen_indicies(secondaryIndex, current_height + 1);
}
// Initialization routine.
void setup(void)
{
	std::cout << "Current Dimensions are: " << dimensions << " by " << dimensions << std::endl;

	OldTime = Clock::now();

	double frequency = 2.f; //0.1 - 64;
	std::int32_t octaves = 2; //1-16

	float fx = dimensions / frequency;
	float fz = dimensions / frequency;

	siv::PerlinNoise terrainNoise(std::random_device{});
	siv::PerlinNoise cloudNoise(std::random_device{});

	// Intialise vertex array
	int i = 0;

	for (int z = 0; z < MAP_SIZE; ++z)
	{
		for (int x = 0; x < MAP_SIZE; ++x)
		{
			terrainVertices[i].coords.x = (float)x;
			terrainVertices[i].coords.y = terrainNoise.accumulatedOctaveNoise2D_0_1(x / fx, z / fz, octaves) * scale_multiplier;
			terrainVertices[i].coords.z = (float)z;
			terrainVertices[i].coords.w = 1.0;

			terrainVertices[i].normals.x = 0.0;
			terrainVertices[i].normals.y = 1.0;
			terrainVertices[i].normals.z = 0.0;

			waterVertices[i].coords.x = (float)x;
			waterVertices[i].coords.y = 30.0f;
			waterVertices[i].coords.z = (float)z;
			waterVertices[i].coords.w = 1.0;

			waterVertices[i].normals.x = 0.0;
			waterVertices[i].normals.y = 0.0;
			waterVertices[i].normals.z = 0.0;

			++i;
		}
	}

	i=0;

	for (int z = 0; z < cloud_size; ++z)
	{
		for (int x = 0; x < cloud_size; ++x)
		{
			cloudVertices[i].coords.x = (float)x;
			cloudVertices[i].coords.y = 205.0f;
			cloudVertices[i].coords.z = (float)z;
			cloudVertices[i].coords.w = 1.0;

			cloudVertices[i].normals.x = cloudNoise.accumulatedOctaveNoise2D_0_1(x / fx, z / fz, octaves * 3);
			cloudVertices[i].normals.y = -1.0;
			cloudVertices[i].normals.z = 0.0;

			++i;
		}
	}

	// Now build the index data 
	i = 0;
	for (int z = 0; z < MAP_SIZE - 1; ++z)
	{
		i = z * MAP_SIZE;
		for (int x = 0; x < MAP_SIZE * 2; x += 2)
		{
			terrainIndexData[z][x] = i;
			waterIndexData[z][x] = i;
			++i;
		}
		for (int x = 1; x < MAP_SIZE * 2 + 1; x += 2)
		{
			terrainIndexData[z][x] = i;
			waterIndexData[z][x] = i;
			++i;
		}
	}
	
	i = 0;
	for (int z = 0; z < cloud_size - 1; ++z)
	{
		i = z * cloud_size;
		for (int x = 0; x < cloud_size * 2; x += 2)
		{
			cloudIndexData[z][x] = i;
			++i;
		}
		for (int x = 1; x < cloud_size * 2 + 1; x += 2)
		{
			cloudIndexData[z][x] = i;
			++i;
		}
	}

	///compute normal vectors for each vertices
	int index1, index2, index3;
	float dot_value;
	vec3 Pt1, Pt2, Pt3, ttVec, edgeVec1, edgeVec2, norVec, upvec;
	upvec.x = 0.0; upvec.y = 1.0; upvec.z = 0.0;

	for (int z = 0; z < MAP_SIZE - 1; ++z)
	{
		for (int x = 0; x < (MAP_SIZE * 2 - 2); ++x)
		{
			index1 = terrainIndexData[z][x];
			index2 = terrainIndexData[z][x + 1];
			index3 = terrainIndexData[z][x + 2];

			Pt1.x = terrainVertices[index1].coords.x;
			Pt1.y = terrainVertices[index1].coords.y;
			Pt1.z = terrainVertices[index1].coords.z;

			Pt2.x = terrainVertices[index2].coords.x;
			Pt2.y = terrainVertices[index2].coords.y;
			Pt2.z = terrainVertices[index2].coords.z;

			Pt3.x = terrainVertices[index3].coords.x;
			Pt3.y = terrainVertices[index3].coords.y;
			Pt3.z = terrainVertices[index3].coords.z;

			edgeVec1 = Pt2 - Pt1;
			edgeVec2 = Pt3 - Pt1;

			if (x % 2 == 1)
				ttVec = cross(edgeVec2, edgeVec1);
			else
				ttVec = cross(edgeVec1, edgeVec2);
			//norVec = normalize(ttVec);
			dot_value = dot(ttVec, upvec);
			if (dot_value < 0.0000001)
				norVec = -ttVec;
			else
				norVec = ttVec;

			terrainVertices[index1].normals = norVec + terrainVertices[index1].normals;
			terrainVertices[index2].normals = norVec + terrainVertices[index2].normals;
			terrainVertices[index3].normals = norVec + terrainVertices[index3].normals;
		}
	}

	///smooth the normal vectors
	int total;
	total = MAP_SIZE * MAP_SIZE;
	for (i = 0; i < (total - 1); ++i)
	{
		ttVec = terrainVertices[i].normals;
		norVec = normalize(ttVec);
		terrainVertices[i].normals = norVec;
	}

	// Generate texture co-ordinates
	i = 0;
	float fTextureU = float(MAP_SIZE) * 0.1f;
	float fTextureV = float(MAP_SIZE) * 0.1f;
	for (int y = 0; y < MAP_SIZE; ++y)
	{
		for (int x = 0; x < MAP_SIZE; ++x)
		{
			float fScaleS = float(x) / float(MAP_SIZE - 1);
			float fScaleT = float(y) / float(MAP_SIZE - 1);
			terrainVertices[i].texcoords = vec2(fTextureU * fScaleS, fTextureV * fScaleT);
			waterVertices[i].texcoords = vec2(fTextureU * fScaleS, fTextureV * fScaleT);
			cloudVertices[i].texcoords = vec2(fTextureU * fScaleS, fTextureV * fScaleT);
			i++;
		}
	}

	//trees
	index = 0;
	for (int i = 0; i < num_trees; ++i) {
		Gen_indicies(i * (verticesPerTreeSegment * trees_complexity) + i * trees_complexity * 4, 0);
	}

	//now vertices
	bool in_water; 
	for (int i = 0; i < num_trees; ++i) {
		
	}
	vec3 middle;
	int random;
	//float half_agnle = angle * 0.5f;
	index = 0;


	for (int i = 0; i < num_trees; ++i){

		in_water = true;
		while (in_water) {
			random = tree_placement(gen);
			if (terrainVertices[random].coords.y > 35.0f && terrainVertices[random].coords.y < 65.0f) {
				middle = terrainVertices[random].coords;
				in_water = false;
			}
		}

		for (int j = 0; j < trees_complexity; ++j)//create base 'cynidicrical' shape
		{
			// Set the coords (1st 4 elements) and a default colour of black (2nd 4 elements) 
			treeVertices[index].coords.x = middle.x + treethickness * cos(tree_edge_angle * j);
			treeVertices[index].coords.y = middle.y;
			treeVertices[index].coords.z = middle.z + treethickness * sin(tree_edge_angle * j);
			treeVertices[index].coords.w = 1.0;

			treeVertices[index].normals.x = cos(tree_edge_angle * j);
			treeVertices[index].normals.y = 0.0;
			treeVertices[index].normals.z = sin(tree_edge_angle * j);

			index++;
		}
		Gen_treeVerts(1, middle, 0);
		middle += vec3(20.0f, 0.0f, 20.0f);
		++current_tree;
	}
	index = 0;

	glClearColor(1.0, 1.0, 1.0, 0.0);

	// Create shader program executable - read, compile and link shaders
	char* vertexShader = readTextFile("vertexShader.glsl");
	vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, (const char**)&vertexShader, NULL);
	glCompileShader(vertexShaderId);

	char* fragmentShader = readTextFile("fragmentShader.glsl");
	fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, (const char**)&fragmentShader, NULL);
	glCompileShader(fragmentShaderId);

	programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	glUseProgram(programId);
	///////////////////////////////////////

	// Create texture ids.
	glGenTextures(texture_n + 1, texture);
	std::string Texs[] = { "sandTex", "grassTex", "snowTex", "gravelTex" }; //waterTex

	// Load the images.
	std::string TexNames[] = {
		 "Textures/sand.png",
		 "Textures/grass.png",
		 "Textures/snow.png",
		 "Textures/gravel.png",
	};


	int width, height;
	unsigned char* data;

	// Bind images.
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
		TexLoc = glGetUniformLocation(programId, Texs[i].c_str());
		glUniform1i(TexLoc, i); //send texture to shader
	}

	std::string skyboxTextures[] =
	{
		"Textures/skyboxright.jpg",
		"Textures/skyboxleft.jpg",
		"Textures/skyboxtop.jpg",
		"Textures/skyboxbottom.jpg",
		"Textures/skyboxfront.jpg",
		"Textures/skyboxback.jpg"
	};

	unsigned int skyTexLoc = glGetUniformLocation(programId, "skyboxTex");
	glUniform1i(skyTexLoc, texture_n); //send texture to shader

	glActiveTexture(GL_TEXTURE0 + texture_n + 1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_n);

	for (unsigned int i = 0; i < 6; i++)
	{
		unsigned char* data = SOIL_load_image(skyboxTextures[i].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		SOIL_free_image_data(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Create vertex array object (VAO) and vertex buffer object (VBO) and associate data with vertex shader.
	glGenVertexArrays(vao_n, vao);
	glGenBuffers(vbo_n, vbo);

	//terrain
	glBindVertexArray(vao[TERRAIN]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[TERRAIN_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(terrainVertices), terrainVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(terrainVertices[0]), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(terrainVertices[0]), (GLvoid*)sizeof(terrainVertices[0].coords));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(terrainVertices[0]), (GLvoid*)(sizeof(terrainVertices[0].coords) + sizeof(terrainVertices[0].normals)));
	glEnableVertexAttribArray(2);

	//water
	glBindVertexArray(vao[WATER]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[WATER_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(waterVertices), waterVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(waterVertices[0]), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(waterVertices[0]), (GLvoid*)sizeof(waterVertices[0].coords));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(waterVertices[0]), (GLvoid*)(sizeof(waterVertices[0].coords) + sizeof(waterVertices[0].normals)));
	glEnableVertexAttribArray(2);

	//tree
	glBindVertexArray(vao[TREE]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[TREE_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(treeVertices), treeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(treeVertices[0]), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(treeVertices[0]), (GLvoid*)sizeof(treeVertices[0].coords));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(treeVertices[0]), (GLvoid*)(sizeof(treeVertices[0].coords) + sizeof(treeVertices[0].normals)));
	glEnableVertexAttribArray(2);

	//skybox
	glBindVertexArray(vao[SKYBOX]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[SKYBOX_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(3);

	//water
	glBindVertexArray(vao[CLOUDS]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[CLOUDS_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cloudVertices), cloudVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(cloudVertices[0]), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(cloudVertices[0]), (GLvoid*)sizeof(cloudVertices[0].coords));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(cloudVertices[0]), (GLvoid*)(sizeof(cloudVertices[0].coords) + sizeof(cloudVertices[0].normals)));
	glEnableVertexAttribArray(2);


	//light
	glUniform3fv(glGetUniformLocation(programId, "light0.ambient"), 1, &light0.ambient[0]);
	glUniform3fv(glGetUniformLocation(programId, "light0.diffuse"), 1, &light0.diffuse[0]);
	glUniform3fv(glGetUniformLocation(programId, "light0.specular"), 1, &light0.specular[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.coords"), 1, &light0.coords[0]);

	///////////////////////////////////////
	//Materials
	//Terrain
	glUniform3fv(glGetUniformLocation(programId, "terrainMaterial.ambient"), 1, &terrainMaterial.ambient[0]);
	glUniform3fv(glGetUniformLocation(programId, "terrainMaterial.diffuse"), 1, &terrainMaterial.diffuse[0]);
	glUniform3fv(glGetUniformLocation(programId, "terrainMaterial.specular"), 1, &terrainMaterial.specular[0]);
	glUniform1f(glGetUniformLocation(programId, "terrainMaterial.shininess"), terrainMaterial.shininess);

	//Water
	glUniform3fv(glGetUniformLocation(programId, "waterMaterial.ambient"), 1, &waterMaterial.ambient[0]);
	glUniform3fv(glGetUniformLocation(programId, "waterMaterial.diffuse"), 1, &waterMaterial.diffuse[0]);
	glUniform3fv(glGetUniformLocation(programId, "waterMaterial.specular"), 1, &waterMaterial.specular[0]);
	glUniform1f(glGetUniformLocation(programId, "waterMaterial.shininess"), waterMaterial.shininess);

	// Obtain projection matrix uniform location and set value.
	projMatLoc = glGetUniformLocation(programId, "projMat");
	projMat = perspective(radians(60.0), (double)SCREEN_WIDTH / (double)SCREEN_HEIGHT, 0.1, 3000.0);
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));

	///////////////////////////////////////
	normalMatLoc = glGetUniformLocation(programId, "modelMat");
	viewPosLoc = glGetUniformLocation(programId, "viewPos");

	objectLoc = glGetUniformLocation(programId, "object");  //uniform uint object;

	// Move terrain into view - glm::translate replaces glTranslatef
	modelViewMat = translate(modelViewMat, vec3(-MAP_SIZE / 2, -4.f, -MAP_SIZE / 2));
	modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

	// Calculate and update normal matrix, after any changes to the view matrix
	glUniformMatrix4fv(normalMatLoc, 1, GL_FALSE, value_ptr(mat4(1.0f)));

	CurrentTime = Clock::now();
	DeltaTime = CurrentTime - OldTime;
	deltaTimefloat = DeltaTime.count() * 0.000001;
	std::cout << "Time taken to generate world in milliseconds: " << deltaTimefloat << std::endl;
}

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//terrain
	glUniform1ui(objectLoc, TERRAIN);
	glBindVertexArray(vao[TERRAIN]);
	for (int i = 0; i < MAP_SIZE - 1; i++)
	{
		glDrawElements(GL_TRIANGLE_STRIP, verticesPerStrip, GL_UNSIGNED_INT, terrainIndexData[i]);
	}

	//water
	glUniform1ui(objectLoc, WATER);
	glBindVertexArray(vao[WATER]);
	for (int i = 0; i < MAP_SIZE - 1; i++)
	{
		glDrawElements(GL_TRIANGLE_STRIP, verticesPerStrip, GL_UNSIGNED_INT, waterIndexData[i]);
	}

	//draw Skybox
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	glUniform1ui(objectLoc, SKYBOX);
	glBindVertexArray(vao[SKYBOX]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default

	//clouds
	glUniform1ui(objectLoc, CLOUDS);
	glBindVertexArray(vao[CLOUDS]);
	for (int i = 0; i < MAP_SIZE - 1; i++)
	{
		glDrawElements(GL_TRIANGLE_STRIP, verticesPerStrip, GL_UNSIGNED_INT, cloudIndexData[i]);
	}

	//draw trees
	glUniform1ui(objectLoc, TREE);
	glBindVertexArray(vao[TREE]);
	glEnable(GL_LINE_WIDTH);
	glLineWidth(10.0f);
	for (int t = 0; t < num_trees; ++t) {
		for (int i = 0; i < verticesPerTreeSegment; ++i) {
			for (int j = 0; j < trees_complexity; ++j) {
				glDrawElements(GL_LINES_ADJACENCY, 4, GL_UNSIGNED_INT, treeIndexData[t][i][j]);
			}
		}
	}
	glDisable(GL_LINE_WIDTH);

	glFlush();

	modelViewMat = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'w':
	case 'W':
		cameraPos += cameraSpeed * cameraFront * deltaTimefloat;
		break;
	case 'a':
	case 'A':
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTimefloat;
		break;
	case 's':
	case 'S':
		cameraPos -= cameraSpeed * cameraFront * deltaTimefloat;
		break;
	case 'd':
	case 'D':
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTimefloat;
		break;
	default:
		break;
	}
}

void mouse_callback(int xpos, int ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.3f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	Camera_yaw += xoffset;
	Camera_pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (Camera_pitch > 89.0f)
		Camera_pitch = 89.0f;
	if (Camera_pitch < -89.0f)
		Camera_pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(Camera_yaw)) * cos(glm::radians(Camera_pitch));
	front.y = sin(glm::radians(Camera_pitch));
	front.z = sin(glm::radians(Camera_yaw)) * cos(glm::radians(Camera_pitch));
	cameraFront = glm::normalize(front);
}

void Update() {
	drawScene();
	waveTime += DeltaTime.count() * 0.000000002;
	cloudTime += DeltaTime.count() * 0.00000000005;
	if (cloudTime > 1.0f){
		cloudTime -= 1.0f;
	}
	glUniform1f(glGetUniformLocation(programId, "waveTime"), waveTime);
	glUniform1f(glGetUniformLocation(programId, "cloudTime"), cloudTime);
	glUniform3fv(glGetUniformLocation(programId, "viewDir"), 1, &cameraFront[0]);

	OldTime = CurrentTime;
	CurrentTime = Clock::now();
	DeltaTime = CurrentTime - OldTime;
	deltaTimefloat = DeltaTime.count() * 0.000000001;
	std::cout << "FPS: " << 1 / deltaTimefloat << std::endl;
}

// Main routine.
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	// Set the version of OpenGL (4.2)
	glutInitContextVersion(4, 2);
	// The core profile excludes all discarded features
	glutInitContextProfile(GLUT_CORE_PROFILE);
	// Forward compatibility excludes features marked for deprecation ensuring compatability with future versions
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("TerrainGeneration");

	glutSetCursor(GLUT_CURSOR_NONE);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set OpenGL to render in wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glewExperimental = GL_TRUE;
	glewInit();

	glutKeyboardUpFunc([](unsigned char key, int x, int y) {
		keys[key] = false;
		});


	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutPassiveMotionFunc(mouse_callback);
	glutIdleFunc(Update);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}