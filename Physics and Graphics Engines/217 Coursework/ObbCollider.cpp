#include "ObbCollider.h"

ObbCollider::ObbCollider(glm::vec3 x, glm::vec3 y, glm::vec3 z)
{
	position = vec3(0.0f);
	type = "obb";

	xradius = x;
	yradius = y;
	zradius = z;

	xlength = glm::length(xradius);
	ylength = glm::length(yradius);
	zlength = glm::length(zradius);

	xnormal = normalize(xradius);
	ynormal = normalize(yradius);
	znormal = normalize(zradius);
	angle_offset = glm::asin(znormal.z);
	height = vec3(0, ylength, 0);
}

ObbCollider::~ObbCollider()
{
}

glm::vec3 ObbCollider::Get_normal(std::string axis)
{
	if (axis == "x") {
		return xnormal;
	}
	else if (axis == "y") {
		return ynormal;
	}
	else if (axis == "z") {
		return znormal;
	}
	return vec3(0.0f);
}

glm::vec3 ObbCollider::Get_radius(std::string axis)
{
	if (axis == "x") {
		return xradius;
	}
	else if (axis == "y") {
		return yradius;
	}
	else if (axis == "z") {
		return zradius;
	}
	return vec3(0.0f);
}

void ObbCollider::Change_collider_size(float x, float y, float z)
{
	xradius = xnormal * x;
	yradius = ynormal * y;
	zradius = znormal * z;

	xlength = x;
	ylength = y;
	zlength = z;
}

void ObbCollider::Update_model_size()
{
	this->model->Resize(xlength, ylength, zlength);
}

void ObbCollider::Update_model_orientation(float o)
{
	this->model->SetAngle(orientation);
}

void ObbCollider::Adjust_orientation(float o)
{
	orientation = radians(o) + angle_offset;
	xnormal = vec3(glm::cos(orientation), 0, glm::sin(orientation));
	xradius = xnormal * xlength;
	znormal = vec3(glm::cos(orientation + glm::half_pi<float>()), 0, glm::sin(orientation + glm::half_pi<float>()));
	zradius = znormal * zlength;
}
