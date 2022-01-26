#include "Svec.h"

Svec3::Svec3()
{
	x = y = z = 0.0f;
}

Svec3::Svec3(float all_axis_value)
{
	x = y = z = all_axis_value;
}

Svec3::Svec3(float x_value, float y_value, float z_value)
{
	x = x_value;
	y = y_value;
	z = z_value;
}

irr::core::vector3df Svec3::Convert_to_irr(Svec3 vec3)
{
	return irr::core::vector3df(vec3.x, vec3.y, vec3.z);
}

btVector3 Svec3::Convert_to_bt(Svec3 vec3)
{
	return btVector3(vec3.x, vec3.y, vec3.z);
}

Svec3 Svec3::Convert_from_irr(irr::core::vector3df irr_vec3)
{
	return Svec3(irr_vec3.X, irr_vec3.Y, irr_vec3.Z);
}

Svec3 Svec3::Convert_from_bt(btVector3 bt_vec3)
{
	return Svec3(bt_vec3.getX(), bt_vec3.getY(), bt_vec3.getZ());
}

std::vector<float> Svec3::get_vector(Svec3 vec3)
{
	return std::vector<float>() = { vec3.x,vec3.y,vec3.z };
}

Svec3 Svec3::multiply(Svec3 first_vec3, Svec3 second_vec3)
{
	return Svec3(first_vec3.x * second_vec3.x, first_vec3.y * second_vec3.y, first_vec3.z * second_vec3.z);
}

Svec3 Svec3::multiply(Svec3 vec3, float scalar)
{
	return Svec3(vec3.x * scalar, vec3.y * scalar, vec3.z * scalar);
}

Svec3 Svec3::normalize()
{
	double denominator = sqrt(x * x + y * y + z * z);
	return Svec3(x/denominator, y/denominator, z/denominator);
}

Svec3 Svec3::cross(Svec3& other)
{
	return Svec3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}

Svec3 Svec3::operator+(const Svec3& vec3)
{
	return Svec3(this->x + vec3.x, this->y + vec3.y, this->z + vec3.z);
}

Svec3 Svec3::operator-(const Svec3& vec3)
{
	return Svec3(this->x - vec3.x, this->y - vec3.y, this->z - vec3.z);
}

Svec3 Svec3::operator*(const float& scalar)
{
	return Svec3(x*scalar, y*scalar, z*scalar);
}

Svec3::~Svec3()
{
}
