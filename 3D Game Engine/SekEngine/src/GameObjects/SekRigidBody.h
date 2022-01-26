#pragma once
#include "btBulletDynamicsCommon.h"

#include "Svec.h"
#include "Smatrix.h"

class SekRigidBody
{
public:
	SekRigidBody(Svec3 Origin_vector = Svec3(0), Svec3 Collider_scale = Svec3(1), Svec3 gameObject_scale = Svec3(1.0f),int Primitive_shape = 1, float object_mass = 1.0f);
	SekRigidBody(Svec3 Origin_vector, Smatrix3 transform_matrix, Svec3 Collider_scale, Svec3 gameObject_scale,int Primitive_shape, float object_mass);
	~SekRigidBody();
	btRigidBody* get_phy_rb();

	Svec3 get_transform_position();
	Svec3 get_transform_offset();
	Svec3 get_rotation_offset();
	Smatrix3 get_transform_matrix();
	Svec3 get_collider_scale();
	Svec3 get_velocity();
	int get_collider_shape();
	float get_mass();

	void set_transform_position(Svec3 new_origin);
	void set_transform_matrix(Smatrix3 new_matrix);
	void set_transform_offset(Svec3 new_offset);
	void set_rotation_offset(Svec3 new_rot_offset);
	void set_collider_scale(Svec3 new_scale); //works effectivelly as scale, but is tied to rigidbody, should not be the same as GO scale, if unsure leave as Svec3(1)
	void set_GO_scale(Svec3 new_scale);
	void set_mass(float new_mass);

	int get_physics_id();
	void set_physics_id(int newId);
private:
	float mass;
	int collider_shape;
	int physics_id;
	Svec3 transform_offset;
	Svec3 rotation_offset;
	Svec3 GO_scale, collider_scale;//gameObject scale tracking
	btRigidBody* phys_rb;
	btTransform phys_transform;
	btMotionState* phys_motion_state;
	btCollisionShape* phys_collider_shape;
};

