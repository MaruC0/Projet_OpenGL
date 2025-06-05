#ifndef TEXTURED_TRIANGLE_H
#define TEXTURED_TRIANGLE_H

#include "triangle.h"
#include "texture.h"

class TexturedTriangle : public Triangle {
public:
    TexturedTriangle(Shader* shader_program, Texture* texture);

    virtual void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;

private:
    GLuint loc_diffuse_map;
    Texture* texture;
};

#endif