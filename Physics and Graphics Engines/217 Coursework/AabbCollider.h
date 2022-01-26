#pragma once

#include "Collider.h"
#include <glm/detail/type_vec2.hpp>

class AabbCollider : public Collider
{
private:
public:
	float xradius, zradius, height;
	float Xmin, Xmax, Ymin, Ymax, Zmin, Zmax;
	AabbCollider(float x, float y, float z);
	~AabbCollider();
	float Get_radius(std::string axis);

	void Change_collider_size(float x, float h, float z);
	void Update_model_size();
};