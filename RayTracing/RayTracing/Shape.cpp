#include "Shape.h"

Shape::Shape()
{
}

Shape::Shape(glm::vec3 cen, glm::vec3 col, float rough, float metal)
{
	centre = cen;
	colour = col;
	roughness = rough;
	metallic = metal;
}

Shape::~Shape()
{
}

glm::vec3 Shape::getCentre()
{
	return centre;
}

glm::vec3 Shape::getColor()
{
	return colour;
}

bool Shape::intersection(glm::vec3 orig, glm::vec3 dir, float& t)
{
	return false;
}

glm::vec3 Shape::Compute_colour(float ambient_intensity, glm::vec3 light_ray_dir, glm::vec3 light_intensity, glm::vec3 intersection_point, glm::vec3 dir, glm::vec3 orig)
{
	return colour;
}
