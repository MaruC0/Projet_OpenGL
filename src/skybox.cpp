#include <iostream>
#include "skybox.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


unsigned int Skybox::loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

Skybox::Skybox(Shader* shader_program, std::vector<std::string> faces) : shader_program_(shader_program->get_id()), textures(faces)
{
    // positions 
    float vertex_buffer_data[] = {
        -1.0f,  1.0f, -1.0f,    // Upper left back 0
        -1.0f, -1.0f, -1.0f,    // Lower left back 1
         1.0f, -1.0f, -1.0f,    // Lower right back 2
         1.0f,  1.0f, -1.0f,    // Upper right back 3
         1.0f,  1.0f,  1.0f,    // Upper right front 4
         1.0f, -1.0f,  1.0f,    // Lower right front 5
        -1.0f, -1.0f,  1.0f,    // Lower left front 6
        -1.0f,  1.0f,  1.0f     // Upper left front 7
    };

    //Cube indices
    GLint indices[] = {
        // Back
        0, 1, 2,
        2, 3, 0,
        // Left
        0, 1, 6,
        6, 7, 0,
        // Top
        0, 3, 4,
        4, 0, 7,
        // Front
        7, 6, 5,
        5, 7, 4,
        // Right
        4, 3, 2,
        2, 4, 5,
        // Bottom
        5, 6, 1,
        1, 2, 5
    };
    
    cubemapTexture = loadCubemap(textures);;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(2, &buffers[0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void Skybox::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
    glDepthFunc(GL_EQUAL);
    glUseProgram(this->shader_program_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glBindVertexArray(VAO);
    
    // Permet d'être "dans" la skybox
    glm::mat4 view_no_translation = glm::mat4(glm::mat3(view));
    GLint loc = glGetUniformLocation(shader_program_, "view");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view_no_translation));

    loc = glGetUniformLocation(shader_program_, "projection");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDepthFunc(GL_LESS);
}

Skybox::~Skybox() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &buffers[0]);
    glDeleteTextures(1, &cubemapTexture);
}

void Skybox::key_handler(int key) {
    return;
}