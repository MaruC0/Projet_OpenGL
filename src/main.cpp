#include "viewer.h"
#include "triangle.h"
#include "node.h"
#include "shader.h"
#include "texture.h"
#include "textured_triangle.h"
#include <string>
#include <GLFW/glfw3.h>

#ifndef SHADER_DIR
#error "SHADER_DIR not defined"
#endif

int main()
{
    // create window, add shaders & scene objects, then run rendering loop
    Viewer viewer(1080,720);

    // get shaders and textures directory
    std::string shader_dir = SHADER_DIR;
    std::string tex_dir = TEX_DIR;

    Shader *color_shader = new Shader(shader_dir + "node.vert", shader_dir + "node.frag");
    Shader* texture_shader = new Shader(shader_dir + "texture.vert", shader_dir + "texture.frag");

    Texture* ground_texture = new Texture(tex_dir + "texture_sol.jpg");

    // Utility functions and variables
    glm::mat4 id_mat = glm::mat4(1.0f); 
    
    auto simple_rotate = [id_mat](float angle, std::string axis) {
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

    auto uniform_scale = [id_mat](auto factor) {
        return glm::scale(id_mat, glm::vec3(factor, factor, factor));
    };

    Node* main_node = new Node(id_mat);

    // Ground
    Shape* ground = new TexturedTriangle(texture_shader, ground_texture);
    Node* ground_node = new Node();
    ground_node->set_Scale(uniform_scale(200.f));
    ground_node->set_Rotate(simple_rotate(90.f, "x"));
    ground_node->set_Translate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f)));

    ground_node->add(ground);
    viewer.scene_root->add(ground_node);

    Shape* triangle_shape = new Triangle(color_shader);
    Node* triangle_node = new Node(id_mat);
    triangle_node->add(triangle_shape);
    main_node->add(triangle_node);

    auto rotate_anim = [&](){
        
        float time = glfwGetTime();
        float angle = time * 40 * 2;
        float scale_factor = (glm::pow(glm::sin(glm::radians(angle*1.2f)), 2) + 4.f) / 5.f;
        triangle_node->set_RotateAnim(simple_rotate(angle, "y"));
        triangle_node->set_ScaleAnim(uniform_scale(scale_factor));
    };

    viewer.scene_root->add(main_node);
    viewer.set_animation_fun(rotate_anim);
    
    viewer.run();
}