#ifndef SKYBOX_H
#define SKYBOX_H

#include "shape.h"
#include "shader.h"
#include <GL/glew.h>

class Skybox{
public:
    GLint shader_program_;
    Skybox(Shader* shader_program, std::vector<std::string> faces);
    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
    void key_handler(int key);
    virtual ~Skybox();

private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    std::vector<std::string> textures;
    unsigned int loadCubemap(std::vector<std::string> faces);
    unsigned int cubemapTexture;
};

#endif
