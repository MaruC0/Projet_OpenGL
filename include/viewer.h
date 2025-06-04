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

class Viewer {
public:
    Viewer(int width=640, int height=480);

    void run();
    void on_key(int key);
    int width;
    int height;

    void set_animation_fun(std::function<void()> fun);

    Node *scene_root;

private:
    GLFWwindow* win;
    std::function<void()> animation_fun;
    static void key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif // VIEWER_H
