#include "textured_triangle.h"

TexturedTriangle::TexturedTriangle(Shader* shader_program, Texture* texture)
    : Triangle(shader_program), texture(texture)
{
    loc_diffuse_map = glGetUniformLocation(this->shader_program_, "diffuse_map");
}

void TexturedTriangle::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {

    glUseProgram(this->shader_program_);

    glActiveTexture(GL_TEXTURE1);
    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, texture->getGLid());
    glUniform1i(loc_diffuse_map, 1);

    Triangle::draw(model, view, projection);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(0);
}