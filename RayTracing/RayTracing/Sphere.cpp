//#include "stdafx.h"
#include "sphere.h"

Sphere::Sphere()
{
}

Sphere::Sphere(float r, glm::vec3 origin, glm::vec3 colour, float rough, float metal) : Shape(origin, colour, rough, metal) {
	radius = r;
}

Sphere::~Sphere()
{
}

float Sphere::getRadius(void)
{
	return radius;
}

glm::vec3 Sphere::getCentre(void)
{
	return centre;
}

bool Sphere::intersection(glm::vec3 orig, glm::vec3 dir, float& t)
{
	float t0, t1; // solutions for t if the ray intersects 

	// geometric solution  // vector dir has to be normalize, length is 1.0
	glm::vec3 L = centre - orig;
	float tca = glm::dot(L, dir);
	if (tca < 0) return false;
	float d = glm::dot(L, L) - tca * tca;
	if (d > (radius * radius)) return false;

	float thc = glm::sqrt(radius * radius - d);
	t0 = tca - thc;
	t1 = tca + thc;

	if (t0 > t1) {
		float temp;
		temp = t0;
		t0 = t1;
		t1 = temp;
	}

	if (t0 < 0) {
		t0 = t1; // if t0 is negative, let's use t1 instead 
		if (t0 < 0) return false; // both t0 and t1 are negative 
	}

	t = t0;
	return true;
}

glm::vec3 Sphere::Compute_colour(float ambient_intensity, glm::vec3 light_ray_dir, glm::vec3 light_intensity, glm::vec3 intersection_point, glm::vec3 dir, glm::vec3 orig)
{

	glm::vec3 reflection, surface_normal;
	float Ca, Cd, Cs; //Ca for ambient colour; //Cd for diffuse colour; //Cs for specular highlights
	float tt;

	//ambient
	Ca = ambient_intensity;

	////deiffuse -- roughness
	surface_normal = glm::normalize(intersection_point - centre);
	Cd = roughness * (float)std::max(0.0, (double)dot(light_ray_dir, surface_normal));

	////specular -- metallic
	reflection = (surface_normal * 2.0f * dot(surface_normal, light_ray_dir)) - light_ray_dir;

	tt = glm::max(0.0, (double)dot(reflection, -dir));
	Cs = metallic * ((float)std::pow(tt, 20));

	return colour * light_intensity * (Ca + Cd + Cs);
}
