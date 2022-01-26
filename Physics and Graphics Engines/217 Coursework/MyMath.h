#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

glm::vec3 find_point_2D(float distance, float angle, glm::vec3 forward_vector = glm::vec3(0,1,0));
glm::vec3 rotate_vector(glm::vec3 v, float angle, char axis);