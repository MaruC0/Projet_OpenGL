#pragma once

#include "texture.h"
#include "shape.h"
#include "shader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

class Ground : public Shape {
public:
    Ground(Shader* shader_program, Texture* texture, glm::vec3 light_position, glm::vec3 light_color, glm::vec3 object_color);
    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
    void key_handler(int key);
    virtual ~Ground();
    void setCamera(glm::vec3 camera_position);

private:
    GLuint VAO;
    GLuint VBO;
    GLuint loc_diffuse_map;
    Texture* texture;
    glm::vec3 light_position;
    glm::vec3 light_color;
    glm::vec3 object_color;
    glm::vec3 camera_position;

    GLint light_pos_loc;
    GLint light_color_loc;
    GLint object_color_loc;
    GLint view_pos_loc;
};