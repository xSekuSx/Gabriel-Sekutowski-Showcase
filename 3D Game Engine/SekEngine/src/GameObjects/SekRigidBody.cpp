#include "SekRigidBody.h"

SekRigidBody::~SekRigidBody()
{
	//phys_collider_shape->~btCollisionShape();
	//phys_motion_state->~btMotionState();
	//phys_rb->~btRigidBody();
}

SekRigidBody::SekRigidBody(Svec3 Origin_vector_offset, Svec3 Collider_scale, Svec3 gameObject_scale, int Primitive_shape, float object_mass)
{
	collider_shape = Primitive_shape;
	mass = object_mass;
	GO_scale = gameObject_scale;
	collider_scale = Collider_scale;
	btVector3 temp_total_size = Svec3::Convert_to_bt(Svec3::multiply(GO_scale, collider_scale));
	if (collider_shape == 1) { //match the collider gameobject primitives values 1 == spehere
		phys_collider_shape = new btSphereShape( temp_total_size.getX());
	}
	else if (collider_shape == 2 || collider_shape == 3) {//match the collider gameobject primitives values 2 & 3 == square and plane
		phys_collider_shape = new btBoxShape(temp_total_size);
	}
	phys_transform.setIdentity();
	phys_transform.setOrigin(Svec3::Convert_to_bt(Origin_vector_offset));

	phys_motion_state = new btDefaultMotionState(phys_transform);
	phys_rb = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(object_mass, phys_motion_state, phys_collider_shape));
	phys_rb->setDamping(btScalar(0.1f), btScalar(0.1f));
}

SekRigidBody::SekRigidBody(Svec3 Origin_vector_offset, Smatrix3 transform_matrix, Svec3 Collider_scale, Svec3 gameObject_scale, int Primitive_shape, float object_mass)
{
	collider_shape = Primitive_shape;
	mass = object_mass;
	GO_scale = gameObject_scale;
	collider_scale = Collider_scale;
	btVector3 temp_total_size = Svec3::Convert_to_bt(Svec3::multiply(GO_scale, collider_scale));
	if (collider_shape == 1) {
		phys_collider_shape = new btSphereShape(temp_total_size.getX());
	}
	else if (collider_shape == 2 || collider_shape == 3) {
		phys_collider_shape = new btBoxShape(temp_total_size);
	}

	phys_transform.setIdentity();
	phys_transform.setOrigin(Svec3::Convert_to_bt(Origin_vector_offset));
	phys_transform.setBasis(btMatrix3x3(Svec3::Convert_to_bt(transform_matrix.row1),
										Svec3::Convert_to_bt(transform_matrix.row2),
										Svec3::Convert_to_bt(transform_matrix.row3)));

	phys_motion_state = new btDefaultMotionState(phys_transform);
	phys_rb = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(object_mass, phys_motion_state, phys_collider_shape));
	phys_rb->setDamping(btScalar(0.1f), btScalar(0.1f));
}

btRigidBody* SekRigidBody::get_phy_rb()
{
	return phys_rb;
}

Svec3 SekRigidBody::get_transform_position()
{
	return Svec3::Convert_from_bt(phys_transform.getOrigin());
}

Svec3 SekRigidBody::get_transform_offset()
{
	return transform_offset;
}

Svec3 SekRigidBody::get_rotation_offset()
{
	return rotation_offset;
}

Smatrix3 SekRigidBody::get_transform_matrix()
{
	return Smatrix3(Svec3::Convert_from_bt(phys_transform.getBasis().getRow(0)),
					Svec3::Convert_from_bt(phys_transform.getBasis().getRow(1)),
					Svec3::Convert_from_bt(phys_transform.getBasis().getRow(2)));
}

Svec3 SekRigidBody::get_collider_scale()
{
	return collider_scale;
}

Svec3 SekRigidBody::get_velocity()
{
	return Svec3::Convert_from_bt(phys_rb->getLinearVelocity());
}

int SekRigidBody::get_collider_shape()
{
	return collider_shape;
}

float SekRigidBody::get_mass()
{
	return mass;
}

void SekRigidBody::set_transform_matrix(Smatrix3 new_matrix)
{
	phys_transform.setBasis(btMatrix3x3(Svec3::Convert_to_bt(new_matrix.row1),
										Svec3::Convert_to_bt(new_matrix.row2),
										Svec3::Convert_to_bt(new_matrix.row3)));
}

void SekRigidBody::set_transform_offset(Svec3 new_offset)
{
	transform_offset = new_offset;
}

void SekRigidBody::set_rotation_offset(Svec3 new_rot_offset)
{
	rotation_offset = new_rot_offset;
}

void SekRigidBody::set_collider_scale(Svec3 new_scale)
{
	collider_scale = new_scale;
	//phys_rb->getCollisionShape()->setLocalScaling(Svec3::Convert_to_bt(Svec3::multiply(collider_scale, GO_scale)));
	//btCollisionWorld::updateSingleAABB(rigidbody);
}

void SekRigidBody::set_GO_scale(Svec3 new_scale)
{
	GO_scale = new_scale;
	//phys_rb->getCollisionShape()->setLocalScaling(Svec3::Convert_to_bt(Svec3::multiply(collider_scale, GO_scale)));
	//btCollisionWorld::updateSingleAABB(rigidbody);
}

void SekRigidBody::set_mass(float new_mass)
{
	mass = new_mass;
}

int SekRigidBody::get_physics_id()
{
	return physics_id;
}

void SekRigidBody::set_physics_id(int newId)
{
	physics_id = newId;
}

void SekRigidBody::set_transform_position(Svec3 new_origin)
{
	phys_transform.setOrigin(Svec3::Convert_to_bt(new_origin));
	phys_motion_state->setWorldTransform(phys_transform);
	phys_rb->setMotionState(phys_motion_state);
}

