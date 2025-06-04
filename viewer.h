#ifndef VIEWER_H
#define VIEWER_H

#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "node.h"
#include "skybox.h"

class Viewer {
public:
    Viewer(int width=640, int height=480);

    void run();
    void on_key(int key);

    Node *scene_root;
    std::string shader_dir;

private:
    GLFWwindow* win;
    static void key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods);
    Shader* skybox_shader;
    Skybox* skybox;
};

#endif // VIEWER_H
