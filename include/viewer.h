#ifndef VIEWER_H
#define VIEWER_H

#include <functional>
#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "node.h"
#include "skybox.h"
#include "ground.h"
#include "texture.h"
#include "camera.h"
// #include "model.h"

class Viewer {
public:
    Viewer(int width=640, int height=480);

    void run();
    void on_key(int key);
    int width;
    int height;

    void add_animation_fun(std::function<void()> fun);

    Node *scene_root;
    std::string shader_dir;
    std::string tex_dir;
    std::string model_dir;

private:
    GLFWwindow* win;
    std::vector<std::function<void()>> animation_funs;
    static void key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods);
    Shader* skybox_shader;
    Skybox* skybox;
    Ground* ground;
    glm::mat4 ground_mat;
};

#endif // VIEWER_H