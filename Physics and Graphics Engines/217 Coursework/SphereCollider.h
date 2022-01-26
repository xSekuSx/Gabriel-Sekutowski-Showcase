#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
private:
	float radius;
public:
	SphereCollider(float r);
	~SphereCollider();
	//bool CollideCheck(Collider*);
	float Get_radius();
	void Change_collider_size(float r);
	void Update_model_size();
};

