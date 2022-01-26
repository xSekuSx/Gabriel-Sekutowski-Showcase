#pragma once
#include "Collider.h"
class PlaneCollider : public Collider
{
private:
	glm::vec3 normal, point_on_plain;
public:
	PlaneCollider(glm::vec3 point, glm::vec3 n, float ori);
	~PlaneCollider();
	glm::vec3 Get_normal();
	void Update_model_size();
	void Update_model_orientation(float o);
};

