#include "Collider.h"
#include "SphereCollider.h"
#include "AabbCollider.h"
#include "PlaneCollider.h"
#include "ObbCollider.h"

std::vector<CollisionInfo> Collider::Collisions_data;

glm::vec3 Collider::Get_position()
{
	return glm::vec3(position);
}

void Collider::Update_model_pos()
{
	model->SetPosition(position);
}

void Collider::Adjust_position(glm::vec3 pos)
{
	position = pos;
}

void Collider::Adjust_orientation(float o)
{
}

void Collider::Attatch_model(Model* m)
{
	model = m;
}

void Collider::Update_model_orientation(float o)
{
}

Model* Collider::Get_model()
{
	return model;
}

#include "glm/gtx/string_cast.hpp"

bool Collider::CollideCheck(Collider* other)
{
	/*
		=========================================================================
		Checking collision against:
		=========================================================================
		Sphere x Sphere:
			checking if distance is smaller than two radii added together

		Sphere x plane:
			finding distance from the plane to the point,
			checking if distance is smaller than the radius

		Sphere x AABB:
			find mid points between min, max, and sphere on all 3 axes
			check distance between that point and sphere

		Sphere x OBB:
			find a direction vector between two objects origin points - dist
			using dot product to calculate the dist and adding according radius
			do it 3 times with those 3 normals

		=========================================================================
		AABB x AABB:
			checking distance between centres, and two radii added together
			3 times for 3 axes

		AABB x plane:
			finding size of the aabb in comparison with the plane, r
			finding distance from plane to centre, s
			checking if s < r

		AABB x OBB:
			finding 4 half diagonals,
				seeing which one is the "longest" in a plane
				checking the positive and negative half diagonal against an axis
				repeat 3 times above two lines

		=========================================================================
		OBB x OBB:
			finding 4 half diagonals,
				seeing which one is the "longest" in a plane
				checking the positive and negative half diagonal against a normal
				repeat 3 times above two lines

		OBB x Plane:
			find distance from plane to obb centre - r
			find 8 corners
				calculate any of the corners dot product with plane normal
				check if the product has a bigger length than r
				
		=========================================================================
	*/

//============================================================================================================================================================================================================================

	if (this->type == "sphere" && other->type == "sphere") {
		SphereCollider* this_temp = dynamic_cast<SphereCollider*>(this);
		SphereCollider* other_temp = dynamic_cast<SphereCollider*>(other);

		glm::vec3 distance_vector = other_temp->Get_position() - this_temp->Get_position();
		float vector_magnitude = distance_vector.x * distance_vector.x + distance_vector.y * distance_vector.y + distance_vector.z * distance_vector.z;
		float sqrt_mag = std::sqrt(vector_magnitude);

		if (sqrt_mag <= (this_temp->Get_radius() + other_temp->Get_radius())) {
			CollisionInfo new_collision = CollisionInfo();
			new_collision.collision_point = distance_vector * 0.5f + this_temp->Get_position();
			new_collision.penetration_depth = ((this_temp->Get_radius() + other_temp->Get_radius()) - sqrt_mag) /2.0f;
			new_collision.collision_normal = glm::normalize(distance_vector);
			Collisions_data.push_back(new_collision);
			return true;
		}
	}

	else if ((this->type == "sphere" && other->type == "plane") || (this->type == "plane" && other->type == "sphere")) {
		SphereCollider* this_temp;
		PlaneCollider* other_temp;
		if (this->type == "sphere") {
			this_temp = dynamic_cast<SphereCollider*>(this);
			other_temp = dynamic_cast<PlaneCollider*>(other);
		}
		else {
			this_temp = dynamic_cast<SphereCollider*>(other);
			other_temp = dynamic_cast<PlaneCollider*>(this);
		}
		glm::vec3 v = (this_temp->Get_position() - other_temp->Get_position());
		float dist = glm::dot(v, other_temp->Get_normal());
		if (abs(dist) < this_temp->Get_radius()) {
			CollisionInfo new_collision = CollisionInfo();
			new_collision.collision_point = this_temp->Get_position() + (dist)*other_temp->Get_normal();
			new_collision.penetration_depth = this_temp->Get_radius() - dist;
			new_collision.collision_normal = other_temp->Get_normal();
			Collisions_data.push_back(new_collision);
			return true;
		}
	}

	else if ((this->type == "sphere" && other->type == "aabb") || (this->type == "aabb" && other->type == "sphere")) {
		SphereCollider* this_temp;
		AabbCollider* other_temp;
		if (this->type == "sphere") {
			this_temp = dynamic_cast<SphereCollider*>(this);
			other_temp = dynamic_cast<AabbCollider*>(other);
		}
		else {
			this_temp = dynamic_cast<SphereCollider*>(other);
			other_temp = dynamic_cast<AabbCollider*>(this);
		}

		//xyz are coords in global 3d space

		float x = fmax((other_temp->position.x - other_temp->xradius), fmin(this_temp->Get_position().x, other_temp->position.x + other_temp->xradius));
		float y = fmax((other_temp->position.y), fmin(this_temp->Get_position().y, other_temp->position.y + other_temp->height));
		float z = fmax((other_temp->position.z - other_temp->zradius), fmin(this_temp->Get_position().z, other_temp->position.z + other_temp->zradius));

		float dist = sqrt((x - this_temp->Get_position().x) * (x - this_temp->Get_position().x)
						+ (y - this_temp->Get_position().y) * (y - this_temp->Get_position().y)
						+ (z - this_temp->Get_position().z) * (z - this_temp->Get_position().z));

		if (dist < this_temp->Get_radius()) {
			CollisionInfo new_collision = CollisionInfo();
			new_collision.collision_point = dist * 0.5f + other_temp->Get_position();
			new_collision.penetration_depth = (dist - this_temp->Get_radius()) / 2;
			new_collision.collision_normal = glm::normalize(vec3(vec3(x, y, z) - other_temp->Get_position()));
			Collisions_data.push_back(new_collision);
			return true;
		}
	}

	else if ((this->type == "sphere" && other->type == "obb") || (this->type == "obb" && other->type == "sphere")) {
		SphereCollider* this_temp;
		ObbCollider* other_temp;
		if (this->type == "sphere") {
			this_temp = dynamic_cast<SphereCollider*>(this);
			other_temp = dynamic_cast<ObbCollider*>(other);
		}
		else {
			this_temp = dynamic_cast<SphereCollider*>(other);
			other_temp = dynamic_cast<ObbCollider*>(this);
		}

		vec3 dist = other_temp->Get_position() - this_temp->Get_position();
		vec3 dir = glm::normalize(dist);
		vec3 r = dir * this_temp->Get_radius();

		if (
			abs(glm::dot(dist, other_temp->Get_normal("x"))) - abs(glm::dot(r, other_temp->Get_normal("x"))) < abs(glm::dot(other_temp->Get_radius("x"), other_temp->Get_normal("x"))) &&
			abs(glm::dot(dist, other_temp->Get_normal("y"))) - abs(glm::dot(r, other_temp->Get_normal("y"))) < abs(glm::dot(other_temp->Get_radius("y"), other_temp->Get_normal("y"))) &&
			abs(glm::dot(dist, other_temp->Get_normal("z"))) - abs(glm::dot(r, other_temp->Get_normal("z"))) < abs(glm::dot(other_temp->Get_radius("z"), other_temp->Get_normal("z")))
			)
		{
			CollisionInfo new_collision = CollisionInfo();
			new_collision.collision_point = this_temp->Get_position() + dist / 2.0f;
			new_collision.penetration_depth = (glm::length(vec3(other_temp->Get_radius("x") + other_temp->Get_radius("y") + other_temp->Get_radius("z"))) - this_temp->Get_radius()) / 20.0f;
			new_collision.collision_normal = dir;
			Collisions_data.push_back(new_collision);
			return true;
		}
	}

//============================================================================================================================================================================================================================

	else if (this->type == "aabb" && other->type == "aabb") {
		AabbCollider* this_temp = dynamic_cast<AabbCollider*>(this);
		AabbCollider* other_temp = dynamic_cast<AabbCollider*>(other);

		vec3 dist = other_temp->Get_position() - this_temp->Get_position();

		if(
		abs(dist.x) < (this_temp->xradius + other_temp->xradius) &&
		abs(dist.z) < (this_temp->zradius + other_temp->zradius) &&
		abs(dist.y) < (this_temp->height + other_temp->height) / 2)
		{

			vec3 this_max = this_temp->Get_position() + vec3(this_temp->xradius, this_temp->height, this_temp->zradius);
			vec3 this_min = this_temp->Get_position() - vec3(this_temp->xradius, 0.0f, this_temp->zradius);

			vec3 other_max = other_temp->Get_position() + vec3(other_temp->xradius, other_temp->height ,other_temp->zradius);
			vec3 other_min = other_temp->Get_position() - vec3(other_temp->xradius, 0.0f, other_temp->zradius);

			std::vector<float>dists{
				(this_max.x - other_min.x),
				(other_max.x - this_min.x),
				(this_max.y - other_min.y),
				(other_max.y - this_min.y),
				(this_max.z - other_min.z),
				(other_max.z - this_min.z),
			};

			float pen = FLT_MAX;
			for (float d : dists) {
				if (d < pen) {
					pen = d;
				}
			}

			//std::cout << "Pen depth: " << pen << std::endl;

			CollisionInfo new_collision = CollisionInfo();
			new_collision.collision_point = dist * 0.5f + this_temp->Get_position();
			new_collision.penetration_depth = pen / 2;
			new_collision.collision_normal = glm::normalize(dist);
			Collisions_data.push_back(new_collision);
			return true;
		}
	}

	else if ((this->type == "aabb" && other->type == "plane") || (this->type == "plane" && other->type == "aabb")) {
		AabbCollider* this_temp;
		PlaneCollider* other_temp;
		if (this->type == "aabb") {
			this_temp = dynamic_cast<AabbCollider*>(this);
			other_temp = dynamic_cast<PlaneCollider*>(other);
		}
		else {
			this_temp = dynamic_cast<AabbCollider*>(other);
			other_temp = dynamic_cast<PlaneCollider*>(this);
		}

		glm::vec3 e = glm::vec3(this_temp->Get_radius("x"), this_temp->Get_radius("y")/2.0f, this_temp->Get_radius("z")); // Compute positive extents

		float r = e.x * abs(other_temp->Get_normal().x) + e.y * abs(other_temp->Get_normal().y) + e.z * abs(other_temp->Get_normal().z); //distance from the plane to the aabb's closest point

		float s = glm::dot(other_temp->Get_normal(),(this_temp->Get_position() + this_temp->height/2 - other_temp->Get_position())); //distance from the plane to the aabb's centre

		if (abs(s) < r) {
			CollisionInfo new_collision = CollisionInfo();
			new_collision.collision_point = this_temp->Get_position() + (r) * other_temp->Get_normal();
			new_collision.penetration_depth = abs(s) - r;
			new_collision.collision_normal = other_temp->Get_normal();
			Collisions_data.push_back(new_collision);
			return true;
		}
	}

	else if ((this->type == "aabb" && other->type == "obb") || (this->type == "obb" && other->type == "aabb")) {
		AabbCollider* this_temp;
		ObbCollider* other_temp;
		if (this->type == "aabb") {
			this_temp = dynamic_cast<AabbCollider*>(this);
			other_temp = dynamic_cast<ObbCollider*>(other);
		}
		else {
			this_temp = dynamic_cast<AabbCollider*>(other);
			other_temp = dynamic_cast<ObbCollider*>(this);
		}

		vec3 x_axis = vec3(1, 0, 0);
		vec3 y_axis = vec3(0, 1, 0);
		vec3 z_axis = vec3(0, 0, 1);

		std::vector<vec3>diagonals{
			other_temp->Get_radius("x") + other_temp->Get_radius("y") + other_temp->Get_radius("z"),
			other_temp->Get_radius("x") + other_temp->Get_radius("y") - other_temp->Get_radius("z"),
			other_temp->Get_radius("x") - other_temp->Get_radius("y") + other_temp->Get_radius("z"),
			other_temp->Get_radius("x") - other_temp->Get_radius("y") - other_temp->Get_radius("z")
		};

		vec3 longest_diagonal = vec3(0);
		float negative;

		for (vec3 d : diagonals) {
			if (abs(glm::dot(d, x_axis)) > abs(glm::dot(longest_diagonal, x_axis))) {
				longest_diagonal = d;
			}
		}

		negative = 1.0f;
		if ((glm::dot(longest_diagonal, x_axis) < 0)) {
			negative *= -1.0f;
		}

		if ((glm::dot(other_temp->Get_position() + longest_diagonal * negative, x_axis)) > (glm::dot(this_temp->Get_position() - this_temp->Get_radius("x"), x_axis)) && (glm::dot(other_temp->Get_position() - longest_diagonal * negative, x_axis)) < (glm::dot(this_temp->Get_position() + this_temp->Get_radius("x"), x_axis))) {

			longest_diagonal = vec3(0);

			for (vec3 d : diagonals) {
				if (abs(glm::dot(d, y_axis)) > abs(glm::dot(longest_diagonal, y_axis))) {
					longest_diagonal = d;
				}
			}

			negative = 1.0f;
			if ((glm::dot(longest_diagonal, y_axis) < 0)) {
				negative *= -1.0f;
			}

			if ((glm::dot(other_temp->Get_position() + longest_diagonal * negative, y_axis)) > (glm::dot(this_temp->Get_position(), y_axis)) && (glm::dot(other_temp->Get_position() - longest_diagonal * negative, y_axis)) < (glm::dot(this_temp->Get_position() + this_temp->Get_radius("y"), y_axis))) {

				longest_diagonal = vec3(0);

				for (vec3 d : diagonals) {
					if (abs(glm::dot(d, z_axis)) > abs(glm::dot(longest_diagonal, z_axis))) {
						longest_diagonal = d;
					}
				}

				negative = 1.0f;
				if ((glm::dot(longest_diagonal, z_axis) < 0)) {
					negative *= -1.0f;
				}

				if ((glm::dot(other_temp->Get_position() + longest_diagonal * negative, z_axis)) > (glm::dot(this_temp->Get_position() - this_temp->Get_radius("z"), z_axis)) && (glm::dot(other_temp->Get_position() - longest_diagonal * negative, z_axis)) < (glm::dot(this_temp->Get_position() + this_temp->Get_radius("z"), z_axis))) {
					//there's technically a collision
					CollisionInfo new_collision = CollisionInfo();
					new_collision.collision_point = this_temp->Get_position() + (this_temp->Get_position() - other_temp->Get_position()) / 2.0f;
					new_collision.penetration_depth = glm::length(abs((this_temp->Get_position() - other_temp->Get_position())))/20.0f;
					new_collision.collision_normal = glm::normalize(this_temp->Get_position() - other_temp->Get_position());
					Collisions_data.push_back(new_collision);
					return true;
				}
			}
		}
	}

//============================================================================================================================================================================================================================

	else if (this->type == "obb" && other->type == "obb") {
		ObbCollider* this_temp = dynamic_cast<ObbCollider*>(this);
		ObbCollider* other_temp = dynamic_cast<ObbCollider*>(other);

		vec3 x_axis = this_temp->Get_normal("x");
		vec3 y_axis = this_temp->Get_normal("y");
		vec3 z_axis = this_temp->Get_normal("z");

		//half diagonals
		std::vector<vec3>diagonals{
			other_temp->Get_radius("x") + other_temp->Get_radius("y") + other_temp->Get_radius("z"),
			other_temp->Get_radius("x") + other_temp->Get_radius("y") - other_temp->Get_radius("z"),
			other_temp->Get_radius("x") - other_temp->Get_radius("y") + other_temp->Get_radius("z"),
			other_temp->Get_radius("x") - other_temp->Get_radius("y") - other_temp->Get_radius("z")
		};

		vec3 longest_diagonal = vec3(0);
		float negative;

		for (vec3 d : diagonals) {
			if (abs(glm::dot(d, x_axis)) > abs(glm::dot(longest_diagonal, x_axis))) {
				longest_diagonal = d;
			}
		}

		negative = 1.0f;
		if ((glm::dot(longest_diagonal, x_axis) < 0)) {
			negative *= -1.0f;
		}

		if ((glm::dot(other_temp->Get_position() + longest_diagonal * negative, x_axis)) > (glm::dot(this_temp->Get_position() - this_temp->Get_radius("x"), x_axis)) && (glm::dot(other_temp->Get_position() - longest_diagonal * negative, x_axis)) < (glm::dot(this_temp->Get_position() + this_temp->Get_radius("x"), x_axis))) {

			longest_diagonal = vec3(0);

			for (vec3 d : diagonals) {
				if (abs(glm::dot(d, y_axis)) > abs(glm::dot(longest_diagonal, y_axis))) {
					longest_diagonal = d;
				}
			}

			negative = 1.0f;
			if ((glm::dot(longest_diagonal, y_axis) < 0)) {
				negative *= -1.0f;
			}

			if ((glm::dot(other_temp->Get_position() + longest_diagonal * negative, y_axis)) > (glm::dot(this_temp->Get_position(), y_axis)) && (glm::dot(other_temp->Get_position() - longest_diagonal * negative, y_axis)) < (glm::dot(this_temp->Get_position() + this_temp->Get_radius("y"), y_axis))) {

				longest_diagonal = vec3(0);

				for (vec3 d : diagonals) {
					if (abs(glm::dot(d, z_axis)) > abs(glm::dot(longest_diagonal, z_axis))) {
						longest_diagonal = d;
					}
				}

				negative = 1.0f;
				if ((glm::dot(longest_diagonal, z_axis) < 0)) {
					negative *= -1.0f;
				}

				if ((glm::dot(other_temp->Get_position() + longest_diagonal * negative, z_axis)) > (glm::dot(this_temp->Get_position() - this_temp->Get_radius("z"), z_axis)) && (glm::dot(other_temp->Get_position() - longest_diagonal * negative, z_axis)) < (glm::dot(this_temp->Get_position() + this_temp->Get_radius("z"), z_axis))) {
					//there's technically a collision
					CollisionInfo new_collision = CollisionInfo();
					new_collision.collision_point = this_temp->Get_position() + (other_temp->Get_position() - this_temp->Get_position()) / 2.0f;
					new_collision.penetration_depth = glm::length(abs((other_temp->Get_position() - this_temp->Get_position()))) / 20;
					new_collision.collision_normal = glm::normalize(other_temp->Get_position() - this_temp->Get_position());
					Collisions_data.push_back(new_collision);
					return true;
				}
			}
		}
	}

	else if ((this->type == "obb" && other->type == "plane") || (this->type == "plane" && other->type == "obb")) {
		ObbCollider* this_temp;
		PlaneCollider* other_temp;
		if (this->type == "obb") {
			this_temp = dynamic_cast<ObbCollider*>(this);
			other_temp = dynamic_cast<PlaneCollider*>(other);
		}
		else {
			this_temp = dynamic_cast<ObbCollider*>(other);
			other_temp = dynamic_cast<PlaneCollider*>(this);
		}

		float s = abs(glm::dot(this_temp->Get_position() - other_temp->Get_position(), other_temp->Get_normal())); // distance parallel to the plane's normal from the plane to the centre of obb

		std::vector<vec3> corners{
			this_temp->Get_radius("x") + this_temp->Get_radius("y") + this_temp->Get_radius("z"),
			this_temp->Get_radius("x") + this_temp->Get_radius("y") - this_temp->Get_radius("z"),
			this_temp->Get_radius("x") - this_temp->Get_radius("y") + this_temp->Get_radius("z"),
			this_temp->Get_radius("x") - this_temp->Get_radius("y") - this_temp->Get_radius("z"),
			-this_temp->Get_radius("x") + this_temp->Get_radius("y") + this_temp->Get_radius("z"),
			-this_temp->Get_radius("x") + this_temp->Get_radius("y") - this_temp->Get_radius("z"),
			-this_temp->Get_radius("x") - this_temp->Get_radius("y") + this_temp->Get_radius("z"),
			-this_temp->Get_radius("x") - this_temp->Get_radius("y") - this_temp->Get_radius("z"),
		}; 

		for (vec3 corner : corners)
		{
			float corner_dot = glm::dot(corner, other_temp->Get_normal());
			if (s < corner_dot) {
				CollisionInfo new_collision = CollisionInfo();
				new_collision.collision_point = this_temp->Get_position() + corner;
				new_collision.penetration_depth = corner_dot - abs(s);
				new_collision.collision_normal = -other_temp->Get_normal();
				Collisions_data.push_back(new_collision);
				return true;
			}
		}
	}

//============================================================================================================================================================================================================================

	return false;
}