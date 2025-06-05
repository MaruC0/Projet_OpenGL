#ifndef VIEWER_H
#define VIEWER_H

#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>

#include "shader.h"
#include "node.h"

class Viewer {
public:
    Viewer(int width=640, int height=480);

    void run();
    void on_key(int key);

    Node *scene_root;

    //Sauvegarde la fonction passée en entrée callback dans updateCallback
    void setUpdateCallback(std::function<void()> callback) {
        updateCallback = callback;
    }

private:
    GLFWwindow* win;
    //Fonction updateCallback utilisée pour stocker une fonction qu'on veut utiliser dans viewer.
    std::function<void()> updateCallback;
    static void key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif // VIEWER_H
