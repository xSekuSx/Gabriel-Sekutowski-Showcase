#include "PlaneCollider.h"

PlaneCollider::PlaneCollider(glm::vec3 point, glm::vec3 n, float ori)
{
	position = vec3(0.0f);
	point_on_plain = point;
	normal = glm::normalize(n);
	type = "plane";
	orientation = ori;
}

PlaneCollider::~PlaneCollider()
{
}

glm::vec3 PlaneCollider::Get_normal()
{
	return normal;
}

void PlaneCollider::Update_model_size()
{

}

void PlaneCollider::Update_model_orientation(float o)
{
	model->SetAxis(orientation, normal);
}
