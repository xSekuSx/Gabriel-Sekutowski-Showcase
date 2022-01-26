#pragma once

#include <glm/gtc/type_ptr.hpp>

class GameObject;

struct CollisionInfo {
	GameObject* first;
	GameObject* second;
	glm::vec3 collision_point, collision_normal;
	float penetration_depth;
	float restitution;
};

struct bin {
	int x, z;
};
