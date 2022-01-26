#pragma once
#include "GameObject.h"

class Particle : public GameObject
{
private:
	float mass;
	glm::vec3 displacement, velocity, acceleration, forces;
public:
	Particle();
	Particle(glm::vec3 dis, float m);
	~Particle();
	void Draw();
	void Update(float);
	void CalculateForces(float);
};