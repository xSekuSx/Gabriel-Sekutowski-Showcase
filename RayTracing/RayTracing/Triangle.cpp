#include "Triangle.h"
//#include <iostream>
//#include <glm/gtx/string_cast.hpp>

Triangle::Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 an, glm::vec3 bn, glm::vec3 cn, glm::vec3 origin, glm::vec3 colour, float rough, float metal) : Shape(origin, colour, rough, metal)
{
	A = a;
	B = b;
	C = c;

	An = an;
	Bn = bn;
	Cn = cn;
}

Triangle::Triangle()
{
}

Triangle::~Triangle()
{
}

bool Triangle::intersection(glm::vec3 orig, glm::vec3 dir, float& t)
{
	glm::vec3 AB = B - A; //v0v1
	glm::vec3 AC = C - A; //v0v2

	float u = (glm::dot((orig - A), (glm::cross(dir, AC)))) / glm::dot(AB, glm::cross(dir, AC));
	float v = (glm::dot(dir, glm::cross(orig - A, AB))) / dot(AB, glm::cross(dir, AC));

	float w = 1 - u - v;

	if (u < 0 || u > 1)
		return false;
	else if (v < 0 || (u + v) > 1)
		return false;
	else
	{
		t = glm::dot(AC, glm::cross((orig - A), AB)) / glm::dot(AB, glm::cross(dir, AC));
		return true;
	}
}

glm::vec3 Triangle::Compute_colour(float ambient_intensity, glm::vec3 light_ray, glm::vec3 light_intensity, glm::vec3 intersection_point, glm::vec3 dir, glm::vec3 orig)
{
	glm::vec3 reflection, ColValue, surface_normal;
	float Ca, Cd, Cs; //Ca for ambient colour; //Cd for diffuse colour; //Cs for specular highlights
	float tt;

	//ambient
	Ca = ambient_intensity;

	////deiffuse -- roughness
	//normal n = wn0 + un1 + v * n2
	glm::vec3 AB = B - A; //v0v1
	glm::vec3 AC = C - A; //v0v2

	float u = (glm::dot((orig - A), (glm::cross(dir, AC)))) / glm::dot(AB, glm::cross(dir, AC));
	float v = (glm::dot(dir, glm::cross(orig - A, AB))) / dot(AB, glm::cross(dir, AC));
	float w = 1 - u - v;
	surface_normal = glm::normalize(w * An + u * Bn + v * Cn);

	Cd = roughness * (float)std::max(0.0, (double)dot(light_ray, surface_normal));

	////specular -- metallic
	reflection = (surface_normal * 2.0f * dot(surface_normal, light_ray)) - light_ray;

	tt = glm::max(0.0, (double)dot(reflection, -dir));
	Cs = metallic * ((float)std::pow(tt, 20));

	ColValue = colour * light_intensity * (Ca + Cd + Cs);
	return ColValue;
}
