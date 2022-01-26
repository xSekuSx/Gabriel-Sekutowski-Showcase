#include "Generator.h"
#include "RigidBody3D.h"

Generator::Generator(vec3 pos, float f, vec3 fd)
{
	position = pos;
	gen_force = f;
	gen_force_dir = fd;
	does_move = false;
	generator = true;
}

Generator::~Generator()
{
}

void Generator::Solve_Interpenetration(GameObject*, float, vec3)
{
}

void Generator::Solve_Impulse(GameObject* other, vec3 normal, vec3 point)
{
	if (!other->does_move) {
		return;
	}
	dynamic_cast<RigidBody3D*>(other)->Add_zone(gen_force, gen_force_dir);
}

void Generator::Update(float delta_time)
{
}

void Generator::Update_once()
{
	if (colliders.size() > 0) {
		for (int i = 0; i < colliders.size(); ++i) {
			colliders[i]->Adjust_position(position);
			colliders[i]->Update_model_orientation(orientation);
		}
	}
}
