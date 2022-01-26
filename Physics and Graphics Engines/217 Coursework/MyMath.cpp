#include "MyMath.h"

glm::vec3 find_point_2D(float distance, float angle, glm::vec3 forward_vector)
{
    return glm::vec3(glm::cos(angle) * distance, 0, glm::sin(angle) * distance);
}

glm::vec3 rotate_vector(glm::vec3 v, float angle, char axis)
{//broken needs fixing
    //offset = vec3(glm::cos(theta - angle) * dist, offset[1], glm::sin(theta - angle) * dist); used instead
    glm::vec3 vec = glm::normalize(v);
    float magnitude = glm::length(v);
    angle = glm::radians(angle);
    if (axis == 'x' || axis == 'X') {
        float theta = glm::acos(vec[0]);
        theta -= angle;
        return glm::vec3(glm::sin(theta) * magnitude, v[1], glm::cos(theta) * magnitude);
    }
    else if (axis == 'y' || axis == 'Y') {
        float phi = glm::acos(v[1]);
        phi += angle;
        return glm::vec3(v[0], glm::cos(phi) * magnitude, v[2]);
    }
    else if (axis == 'z' || axis == 'Z') {
        float theta = glm::acos(v[2]);
        theta -= angle;
        return glm::vec3(glm::cos(theta), v[1], glm::sin(theta));
    }
}
