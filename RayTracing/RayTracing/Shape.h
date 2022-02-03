#pragma once
#include <glm/glm.hpp>
#include <algorithm>



struct light_ray {
	glm::vec3 dir;
	glm::vec3 colour;
	light_ray(glm::vec3 d, glm::vec3 c) {
		dir = d;
		colour = c;
	}
};

class Shape
{
protected:
	glm::vec3 centre;
	glm::vec3 colour;
	float roughness;
public:
	float metallic;
	Shape();
	Shape(glm::vec3, glm::vec3, float, float);
	~Shape();
	glm::vec3 getCentre();
	virtual glm::vec3 getColor();
	virtual bool intersection(glm::vec3 orig, glm::vec3 dir, float& t) = 0;
	virtual glm::vec3 Compute_colour(float ambient_intensity, glm::vec3 light_ray_dir, glm::vec3 light_intensity, glm::vec3 intersection_point, glm::vec3 dir, glm::vec3 orig);
};

