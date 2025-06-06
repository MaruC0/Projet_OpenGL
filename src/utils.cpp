#include "utils.h"

// Utility functions and variables
glm::mat4 id_mat = glm::mat4(1.0f);

glm::mat4 simple_rotate(float angle, std::string axis) {
    glm::vec3 vec;
    if (axis == "x") {
        vec = glm::vec3(1.f, 0.f, 0.f);
    }
    else if (axis == "y") {
        vec = glm::vec3(0.f, 1.f, 0.f);
    }
    else if (axis == "z") {
        vec = glm::vec3(0.f, 0.f, 1.f);
    }
    return glm::rotate(id_mat, glm::radians(angle), vec);
};

glm::mat4 uniform_scale(double factor) {
    return glm::scale(id_mat, glm::vec3(factor, factor, factor));
};