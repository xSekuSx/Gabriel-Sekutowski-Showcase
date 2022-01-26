#pragma once
#include "Model.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <vector>
#include <cmath>
#include <iostream>

class SphereCollider;
class AabbCollider;
class PlaneCollider;
class ObbCollider;
class GameObject;

struct CollisionInfo {
	GameObject* first;
	GameObject* second;
	glm::vec3 collision_point, collision_normal;
	float penetration_depth;
	float restitution;
};

struct Bin {
	int x, z;

	bool operator<(const Bin& rhs) const noexcept
	{
		// logic here
		return this->x < rhs.x; // for example
	}
};

class Collider
{
protected:
	glm::vec3 position;
	glm::vec3 offset;
	Model* model;
	float orientation;
public:
	std::string type;
	static std::vector<CollisionInfo> Collisions_data;
	vec3 diagonal;
	std::vector<Bin> bins;

	virtual glm::vec3 Get_position();
	virtual bool CollideCheck(Collider*);
	virtual void Update_model_pos();
	virtual void Adjust_position(glm::vec3);
	virtual void Adjust_orientation(float);
	virtual void Attatch_model(Model*);
	virtual void Update_model_size() = 0;
	virtual void Update_model_orientation(float);
	virtual Model* Get_model();
};

