#pragma once
#include <irrlicht.h>
#include <vector>
#include "btBulletDynamicsCommon.h"

class Svec3
{
public:
	float x, y, z;

	static irr::core::vector3df Convert_to_irr(Svec3 vec3);
	static btVector3 Convert_to_bt(Svec3 vec3);
	static Svec3 Convert_from_irr(irr::core::vector3df vec3);
	static Svec3 Convert_from_bt(btVector3 vec3);
	static std::vector<float> get_vector(Svec3 vec3);
	static Svec3 multiply(Svec3 first_vec3, Svec3 second_vec3);
	static Svec3 multiply(Svec3 vec3, float scalar);

	Svec3 normalize();
	Svec3 cross(Svec3& vec3);
	Svec3 operator+(const Svec3& vec3);
	Svec3 operator-(const Svec3& vec3);

	Svec3 operator*(const float& scalar);

	Svec3();
	Svec3(float all_axis_value);
	Svec3(float x, float y, float z);
	~Svec3();
};

