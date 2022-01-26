#include "Particle.h"

Particle::Particle()
{
}

Particle::Particle(glm::vec3 dis, float m)
{
	displacement = dis;
	mass = m;

	forces = glm::vec3(0.0f, 0.0f, 0.0f);
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
}

Particle::~Particle()
{
}

void Particle::Draw()
{
	glPushMatrix();
	glColor3f(0.f, 1.f, 0.f);
	glTranslatef(displacement.x, displacement.y, displacement.z);
	glutSolidSphere(0.5f, 10, 10);
	glPopMatrix();

	if (debugging) {
		glPushMatrix();
		glColor3f(1.f, 0.f, 1.f);
		glBegin(GL_LINES);
		glVertex3f(displacement.x, displacement.y, displacement.z);
		glVertex3f(displacement.x + velocity.x, displacement.y + velocity.y, displacement.z + velocity.z);
		glEnd();
		glPopMatrix();
	}
}

void Particle::Update(float deltaTime)
{
	CalculateForces(deltaTime);
}

void Particle::CalculateForces(float delta_time)
{
	forces = glm::vec3(0, 0, 0);
	forces += glm::vec3(0, -9.8f, 0) * mass;
	acceleration = forces / mass;
	displacement = displacement + velocity * delta_time;
	velocity = velocity + acceleration * delta_time;
}
