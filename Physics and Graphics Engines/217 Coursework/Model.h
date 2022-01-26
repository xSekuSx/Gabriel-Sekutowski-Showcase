#pragma once

#include <string>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "vertex.h"
#include "MyMath.h"

using namespace glm;

class Model
{
private:
	int NumVert;

	VertexWithAll* VerticesData;  //Data with vertices, normal, texCoords

	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO; //for triangle indices buffer

	mat4 ModelMatrix;

	glm::vec3 position;
	glm::vec3 offset;
	glm::vec3 off_norm;
	glm::vec3 scale;
	float angle;
	float theta;
	float rotational_offset;
	float rotation;
	float dist;
	int vao_id, vbo_id;

	float orientation;
	glm::vec3 orientation_axis;

	void CreateObject(const char*);

public:
	Model();
	Model(const char*, glm::vec3 s, glm::vec3 off, float roto, int vaoid, int vboid);
	~Model();

	void SetPosition(vec3 newPos);
	void SetAngle(float a);
	void SetAxis(float ori, vec3 axis);
	void AddOffset(float offset);
	void Rotate(float amount);
	vec3 GetPosition(void);

	void SetIDs(unsigned int, unsigned int, unsigned int);
	void updateModelMatrix(unsigned int, vec3);

	void Setup();
	void Draw();
	void Update(float, glm::vec3 offset);
	int Getid(std::string id);

	void Resize(float x, float y, float z);
};

