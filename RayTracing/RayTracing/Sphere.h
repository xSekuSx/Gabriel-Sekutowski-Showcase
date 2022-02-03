#pragma once
#include "shape.h"

class Sphere : public Shape
{
private:
	float radius;
public:
	Sphere();
	Sphere(float radius, glm::vec3 origin, glm::vec3 colour, float rough, float metal);
	~Sphere();
	float getRadius(void);
	glm::vec3 getCentre(void);
	bool intersection(glm::vec3 orig, glm::vec3 dir, float& t) override;
	glm::vec3 Compute_colour(float ambient_intensity, glm::vec3 light_ray_dir, glm::vec3 light_intensity, glm::vec3 intersection_point, glm::vec3 dir, glm::vec3 orig) override;
};