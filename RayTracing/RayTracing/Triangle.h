#pragma once
#include "Shape.h"
class Triangle : public Shape
{
public:
	glm::vec3 A, B, C; //vertecies A,B,C
	glm::vec3 An, Bn, Cn; //normals of A,B,C
	Triangle();
	Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 an, glm::vec3 bn, glm::vec3 cn, glm::vec3 origin, glm::vec3 colour, float rough, float metal);
	~Triangle();
	virtual bool intersection(glm::vec3 orig, glm::vec3 dir, float& t) override;
	glm::vec3 Compute_colour(float ambient_intensity, glm::vec3 light_ray_dir, glm::vec3 light_intensity, glm::vec3 intersection_point, glm::vec3 dir, glm::vec3 orig) override;
};