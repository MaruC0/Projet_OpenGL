#include "ground.h"

Ground::Ground(Shader* shader_program, Texture* texture, glm::vec3 light_position, glm::vec3 light_color, glm::vec3 object_color)
        : Shape(shader_program),
        texture(texture),
        light_position(light_position),
        light_color(light_color),
        object_color(object_color)
{
    loc_diffuse_map = glGetUniformLocation(this->shader_program_, "diffuse_map");
    light_pos_loc = glGetUniformLocation(this->shader_program_, "lightPos");
    light_color_loc = glGetUniformLocation(this->shader_program_, "lightColor");
    object_color_loc = glGetUniformLocation(this->shader_program_, "objectColor");
    view_pos_loc = glGetUniformLocation(this->shader_program_, "viewPos");

    GLfloat vertex_buffer_data[] = {
        -1.0f, 0.0f, -1.0f, 0.f, 1.f, 0.f, 1.0f, 1.0f,
        -0.5f, 0.0f, -1.0f, 0.f, 1.f, 0.f, 0.0f, 1.0f,
        -1.0f, 0.0f, -0.5f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        -1.0f, 0.0f, -0.5f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, 0.f, 1.f, 0.f, 0.0f, 0.0f,
        -0.5f, 0.0f, -1.0f, 0.f, 1.f, 0.f, 0.0f, 1.0f,

        -0.5f, 0.0f, -1.0f, 0.f, 1.f, 0.f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.0f, 0.f, 1.f, 0.f, 0.0f, 1.0f,
        -0.5f, 0.0f, -0.5f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        0.0f, 0.0f, -0.5f, 0.f, 1.f, 0.f, 0.0f, 0.0f,
        0.0f, 0.0f, -1.0f, 0.f, 1.f, 0.f, 0.0f, 1.0f,

        0.0f, 0.0f, -1.0f, 0.f, 1.f, 0.f, 1.0f, 1.0f,
        0.5f, 0.0f, -1.0f, 0.f, 1.f, 0.f, 0.0f, 1.0f,
        0.0f, 0.0f, -0.5f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        0.0f, 0.0f, -0.5f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        0.5f, 0.0f, -0.5f, 0.f, 1.f, 0.f, 0.0f, 0.0f,
        0.5f, 0.0f, -1.0f, 0.f, 1.f, 0.f, 0.0f, 1.0f,

        0.5f, 0.0f, -1.0f, 0.f, 1.f, 0.f, 1.0f, 1.0f,
        1.0f, 0.0f, -1.0f, 0.f, 1.f, 0.f, 0.0f, 1.0f,
        0.5f, 0.0f, -0.5f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        0.5f, 0.0f, -0.5f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        1.0f, 0.0f, -0.5f, 0.f, 1.f, 0.f, 0.0f, 0.0f,
        1.0f, 0.0f, -1.0f, 0.f, 1.f, 0.f, 0.0f, 1.0f,


        -1.0f, 0.0f, -0.5f, 0.f, 1.f, 0.f, 1.0f, 1.0f,
        -0.5f, 0.0f, -0.5f, 0.f, 1.f, 0.f, 0.0f, 1.0f,
        -1.0f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        -1.0f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        -0.5f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 0.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, 0.f, 1.f, 0.f, 0.0f, 1.0f,

        -0.5f, 0.0f, -0.5f, 0.f, 1.f, 0.f, 1.0f, 1.0f,
        0.0f, 0.0f, -0.5f, 0.f, 1.f, 0.f, 0.0f, 1.0f,
        -0.5f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        -0.5f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 0.0f, 0.0f,
        0.0f, 0.0f, -0.5f, 0.f, 1.f, 0.f, 0.0f, 1.0f,

        0.0f, 0.0f, -0.5f, 0.f, 1.f, 0.f, 1.0f, 1.0f,
        0.5f, 0.0f, -0.5f, 0.f, 1.f, 0.f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        0.5f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 0.0f, 0.0f,
        0.5f, 0.0f, -0.5f, 0.f, 1.f, 0.f, 0.0f, 1.0f,

        0.5f, 0.0f, -0.5f, 0.f, 1.f, 0.f, 1.0f, 1.0f,
        1.0f, 0.0f, -0.5f, 0.f, 1.f, 0.f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        0.5f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 0.0f, 0.0f,
        1.0f, 0.0f, -0.5f, 0.f, 1.f, 0.f, 0.0f, 1.0f,


        -1.0f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 1.0f, 1.0f,
        -0.5f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 0.0f, 1.0f,
        -1.0f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        -1.0f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        -0.5f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 0.0f, 0.0f,
        -0.5f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 0.0f, 1.0f,

        -0.5f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 0.0f, 1.0f,
        -0.5f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        -0.5f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 0.0f, 1.0f,

        0.0f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 1.0f, 1.0f,
        0.5f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 0.0f, 1.0f,

        0.5f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 0.0f, 1.0f,


        -1.0f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 1.0f, 1.0f,
        -0.5f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 0.0f, 1.0f,
        -1.0f, 0.0f, 1.0f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        -1.0f, 0.0f, 1.0f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        -0.5f, 0.0f, 1.0f, 0.f, 1.f, 0.f, 0.0f, 0.0f,
        -0.5f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 0.0f, 1.0f,

        -0.5f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 0.0f, 1.0f,
        -0.5f, 0.0f, 1.0f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        -0.5f, 0.0f, 1.0f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.f, 1.f, 0.f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 0.0f, 1.0f,

        0.0f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 1.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        0.5f, 0.0f, 1.0f, 0.f, 1.f, 0.f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 0.0f, 1.0f,

        0.5f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 0.0f, 1.0f,
        0.5f, 0.0f, 1.0f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        0.5f, 0.0f, 1.0f, 0.f, 1.f, 0.f, 1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.f, 1.f, 0.f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 0.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));


}

Ground::~Ground() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Ground::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {

    glUseProgram(this->shader_program_);

    // activate, bind the texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture->getGLid());

    // send the correct texture to the shader
    glUniform1i(loc_diffuse_map, 1);

    glBindVertexArray(VAO);

    Shape::draw(model, view, projection);

    glDrawArrays(GL_TRIANGLES, 0, 96);

    glUniform3fv(light_pos_loc, 1, glm::value_ptr(light_position));
    glUniform3fv(light_color_loc, 1, glm::value_ptr(light_color));
    glUniform3fv(object_color_loc, 1, glm::value_ptr(object_color));
    glUniform3fv(view_pos_loc, 1, glm::value_ptr(camera_position));

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(0);
}

void Ground::key_handler(int key) {
    return;
}

void Ground::setCamera(glm::vec3 camera_pos) {
    camera_position = camera_pos;
}