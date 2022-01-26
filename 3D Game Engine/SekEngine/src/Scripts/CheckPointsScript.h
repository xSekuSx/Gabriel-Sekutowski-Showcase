#pragma once
#include "Scripting.h"
#include "GameObject.h"
#include "inputSystem.h"
#include "math.h"

class CheckPointsScript : Scripting {
public:
	float CalculateDistance(GameObject* first_obj, GameObject* second_obj) {
		Svec3 diff = first_obj->get_position() - second_obj->get_position();
		return sqrtf(diff.x * diff.x + diff.y * diff.y + diff.z + diff.z);
	}

	void Initialise() {
		NewScript();
	};

	void Update() {
		if (InputSystem::keys[input_key::USE].is_pressed) {
			std::cout << "pressed" << std::endl;
		}
	}
};