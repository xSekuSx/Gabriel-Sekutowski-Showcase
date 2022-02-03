#pragma once
#include "shape.h"
class Plane : public Shape
{
public:
	glm::vec3 normal;
	Plane();
	Plane(glm::vec3 norm, glm::vec3 origin, glm::vec3 colour, float rough, float metal);
	~Plane();
	bool intersection(glm::vec3 orig, glm::vec3 dir, float& t) override;
	glm::vec3 Compute_colour(float ambient_intensity, glm::vec3 light_ray_dir, glm::vec3 light_intensity, glm::vec3 intersection_point, glm::vec3 dir, glm::vec3 orig) override;
};