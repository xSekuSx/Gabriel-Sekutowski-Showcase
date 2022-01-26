#include "AabbCollider.h"
#include "SphereCollider.h"

AabbCollider::AabbCollider(float x, float h, float z)
{
	position = vec3(0.0f);
	xradius = x;
	height = h;
	zradius = z;
	type = "aabb";
}

AabbCollider::~AabbCollider()
{
}

float AabbCollider::Get_radius(std::string axis)
{
	if (axis == "x") {
		return xradius;
	}
	else if (axis == "y") {
		return height;
	}
	else if (axis == "z") {
		return zradius;
	}
	return 0.0f;
}

void AabbCollider::Change_collider_size(float x, float h, float z)
{
	if (x > 0) {
		xradius = x;
	}
	if (h > 0) {
		height = h;
	}
	if (z > 0) {
		zradius = z;
	}
}

void AabbCollider::Update_model_size()
{
	model->Resize(xradius, height/2.0f, zradius);
}
