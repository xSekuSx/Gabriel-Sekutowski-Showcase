#pragma once

#include "Collider.h"

class ObbCollider : public Collider
{
private:
	glm::vec3 xradius, yradius, zradius;
	glm::vec3 xnormal, ynormal, znormal;
	float xlength, ylength, zlength;
	float angle_offset;
	vec3 height;
public:
	ObbCollider(glm::vec3 x, glm::vec3 y, glm::vec3 z);
	~ObbCollider();

	glm::vec3 Get_normal(std::string);
	glm::vec3 Get_radius(std::string);
	void Change_collider_size(float x, float y, float z);
	void Update_model_size();
	void Update_model_orientation(float o);
	void Adjust_orientation(float o);
};