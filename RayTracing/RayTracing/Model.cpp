#include "Model.h"

Model::Model()
{
	position = vec3(0);
	VAO = VBO = 0;
	VerticesData = NULL;
	NumVert = 0;
	angle = 0;
	scale = vec3(0);
	offset = vec3(0);
	off_norm = vec3(0);
	theta = 0;
	dist = 0;
	rotation = 0;
	rotational_offset = 0;
	vao_id = 0;
	vbo_id = 0;
	orientation = 0;
	orientation_axis = vec3(0, 1, 0);
}

Model::Model(const char* name, vec3 pos)
{
	position = pos;
	VAO = VBO = 0;
	VerticesData = NULL;
	NumVert = 0;
	CreateObject(name);
	angle = 0;
	//scale = s;
	//offset = off;
	//off_norm = glm::normalize(off);
	theta = glm::asin(off_norm[2]);
	//dist = glm::length(off);
	rotation = 0;
	//rotational_offset = glm::radians(roto);
	//vao_id = vaoid;
	//vbo_id = vboid;
	orientation = 0;
	orientation_axis = vec3(0, 1, 0);
}

Model::~Model()
{
	//free(VerticesData);
	delete[] VerticesData;
}

void Model::CreateObject(const char* name)
{
	std::vector<VertexWithAll> mesh = loadOBJ(name); //Import OBJ file data into a vector array
	NumVert = mesh.size();

	this->VerticesData = new VertexWithAll[NumVert];
	for (size_t i = 0; i < NumVert; i++)
	{
		this->VerticesData[i] = mesh[i];
	}
}

void Model::SetPosition(vec3 newPos)
{
	position = newPos;
}

vec3 Model::GetPosition(void)
{
	return position;
}

void Model::SetAngle(float a) {
	angle = a;
	if (offset != vec3(0)) { //offset is basically hypotenuse
		offset = vec3(glm::cos(rotational_offset - rotation / 16.0f - angle + theta) * -dist, offset[1], glm::sin(rotational_offset - rotation / 16.0f - angle + theta) * -dist); //why the f do I didvide by 16?
	}
}

void Model::SetAxis(float ori, vec3 axis)
{
	orientation = ori;
	orientation_axis = vec3(1, 0, 1) - abs(axis);
}

void Model::AddOffset(float off)
{
	rotational_offset += off;
}

void Model::Rotate(float amount)
{
	rotation = amount;
}

void Model::SetIDs(unsigned int vao, unsigned int vbo, unsigned int ibo)
{
	VAO = vao;
	VBO = vbo;
	IBO = ibo;
}

void Model::Setup()
{
	//glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(VertexWithAll) * NumVert, VerticesData, GL_STATIC_DRAW);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexWithAll), (GLvoid*)offsetof(VertexWithAll, position));
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexWithAll), (GLvoid*)offsetof(VertexWithAll, normal));
	//glEnableVertexAttribArray(1);

	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexWithAll), (GLvoid*)offsetof(VertexWithAll, textcoord));
	//glEnableVertexAttribArray(2);
}

void Model::updateModelMatrix(unsigned int modelMatLoc, vec3 camera)
{
	//mat4 ModelMatrix = glm::mat4(1.0f);
	//ModelMatrix = glm::translate(ModelMatrix, position + offset);
	//ModelMatrix = glm::rotate(ModelMatrix, angle, vec3(0, 1, 0));
	//ModelMatrix = glm::rotate(ModelMatrix, rotational_offset + rotation, vec3(0, 1, 0));
	//if (orientation) {
	//	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(orientation), orientation_axis);
	//}
	//ModelMatrix = glm::scale(ModelMatrix, scale);
	//glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, value_ptr(ModelMatrix));  //send modelview matrix to the shader
}

void Model::Draw()
{
	//glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, NumVert);
}

void Model::Update(float deltaTime, glm::vec3 offset)
{
	//collider->Update(deltaTime, position, offset);
}

int Model::Getid(std::string id)
{
	if (id == "vao") {
		return vao_id;
	}
	else if (id == "vbo") {
		return vbo_id;
	}
	return 0;
}

void Model::Resize(float x, float y, float z)
{
	if (x > 0) {
		scale.x = x;
	}
	if (y > 0) {
		scale.y = y;
	}
	if (z > 0) {
		scale.z = z;
	}
}

bool Model::intersection(glm::vec3 orig, glm::vec3 dir, float& t)
{
	//for (int i = 0; i < NumVert; i++) {
	//	Triangle::intersection(orig, dir, t);
	//	VerticesData->position
	//}
	return false;
}
