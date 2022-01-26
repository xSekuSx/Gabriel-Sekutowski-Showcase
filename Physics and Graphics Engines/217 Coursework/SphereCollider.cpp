#include "SphereCollider.h"


SphereCollider::SphereCollider(float r)
{
	position = vec3(0.0f);
	radius = r;
	type = "sphere";
}

SphereCollider::~SphereCollider()
{
}

float SphereCollider::Get_radius()
{
	return radius;
}

void SphereCollider::Change_collider_size(float r) {
	radius = r;
}

void SphereCollider::Update_model_size()
{
	model->Resize(radius, radius, radius);
}
