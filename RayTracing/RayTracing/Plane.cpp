#include "Plane.h"

Plane::Plane()
{
}

Plane::~Plane()
{
}

Plane::Plane(glm::vec3 norm, glm::vec3 origin, glm::vec3 colour, float rough, float metal) : Shape(origin, colour, rough, metal)
{
    normal = norm;
}


bool Plane::intersection(glm::vec3 orig, glm::vec3 dir, float& t)
{
    float denom = glm::dot(dir, normal);
    if (denom > 0.0f) {
        return false;
    }
    t = glm::dot((centre - orig), normal) / denom;
    return true;
}

glm::vec3 Plane::Compute_colour(float ambient_intensity, glm::vec3 light_ray_dir, glm::vec3 light_intensity, glm::vec3 intersection_point, glm::vec3 dir, glm::vec3 orig)
{
	glm::vec3 reflection, ColValue;
	float Ca, Cd, Cs; //Ca for ambient colour; //Cd for diffuse colour; //Cs for specular highlights
	float tt;

	//ambient
	Ca = ambient_intensity;

	////deiffuse -- roughness
	Cd = roughness * (float)std::max(0.0, (double)dot(light_ray_dir, normal));

	////specular -- metallic
	reflection = (normal * 2.0f * dot(normal, light_ray_dir)) - light_ray_dir;

	tt = glm::max(0.0, (double)dot(reflection, -dir));
	Cs = metallic * ((float)std::pow(tt, 20));

	ColValue = colour * light_intensity * (Ca + Cd + Cs);
	return ColValue;
}
