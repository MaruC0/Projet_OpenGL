#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Utility functions and variables
extern glm::mat4 id_mat;
    
glm::mat4 simple_rotate(float angle, std::string axis);
glm::mat4 uniform_scale(double factor);