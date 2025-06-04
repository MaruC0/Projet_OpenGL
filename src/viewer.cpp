#include "viewer.h"
#include "skybox.h"

#include <functional>
#include <iostream>
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>

Viewer::Viewer(int width, int height)
{
    this->width = width;
    this->height = height;

    if (!glfwInit())    // initialize window system glfw
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        glfwTerminate();
    }

    // version hints: create GL window with >= OpenGL 3.3 and core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    win = glfwCreateWindow(width, height, "Viewer", NULL, NULL);

    if (win == NULL) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
    }

    // make win's OpenGL context current; no OpenGL calls can happen before
    glfwMakeContextCurrent(win);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
    }

    // Set user pointer for GLFW window to this Viewer instance
    glfwSetWindowUserPointer(win, this);

    // register event handlers
    glfwSetKeyCallback(win, key_callback_static);
    
    // Hide the mouse and enable unlimited movement
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(win, width/2.f, height/2.f);

    // useful message to check OpenGL renderer characteristics
    std::cout << glGetString(GL_VERSION) << ", GLSL "
              << glGetString(GL_SHADING_LANGUAGE_VERSION) << ", Renderer "
              << glGetString(GL_RENDERER) << std::endl;

    // initialize GL by setting viewport and default render characteristics
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

    /* tell GL to only draw onto a pixel if the shape is closer to the viewer
    than anything already drawn at that pixel */
    glEnable( GL_DEPTH_TEST ); /* enable depth-testing */
    /* with LESS depth-testing interprets a smaller depth value as meaning "closer" */
    glDepthFunc( GL_LESS );

    // initialize our scene_root
    scene_root = new Node();
    /*
    // get shader directory
    shader_dir = SHADER_DIR;

    // initialize skybox
    skybox_shader = new Shader(shader_dir + "skybox.vert", shader_dir + "skybox.frag");
    std::vector<std::string> faces =
    {
    "C:/Users/jarva/Code/OpenGL/Projet Jardin/material/textures/right.png",
    "C:/Users/jarva/Code/OpenGL/Projet Jardin/material/textures/left.png",
    "C:/Users/jarva/Code/OpenGL/Projet Jardin/material/textures/top.png",
    "C:/Users/jarva/Code/OpenGL/Projet Jardin/material/textures/bottom.png",
    "C:/Users/jarva/Code/OpenGL/Projet Jardin/material/textures/front.png",
    "C:/Users/jarva/Code/OpenGL/Projet Jardin/material/textures/back.png"
    };
    skybox = new Skybox(skybox_shader, faces);*/
}



void Viewer::run()
{
    // Main render loop for this OpenGL window
    while (!glfwWindowShouldClose(win))
    {
        // clear draw buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 id_mat = glm::mat4(1.f);

        glm::mat4 scale = glm::scale(id_mat, glm::vec3(1.F, 1.F, 1.F));
        glm::mat4 rotate = glm::mat4(1.f);
        glm::mat4 translate = glm::translate(id_mat, glm::vec3(0.F, 0.F, 0.F));
        glm::mat4 placement = translate * rotate;

        glm::mat4 view = glm::lookAt(glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 1.f));
        float ratio = static_cast<float>(width)/height;
        glm::mat4 projection = glm::perspective(glm::radians(45.f), ratio, 0.1f, 20.0f);

        // skybox->draw(model, view, projection);

        // Anime le modèle s'il a une fonction d'animation
        if(animation_fun) animation_fun();

        scene_root->draw(placement, scale, view, projection);

        // Poll for and process events
        glfwPollEvents();

        // flush render commands, and swap draw buffers
        glfwSwapBuffers(win);
    }

    /* close GL context and any other GLFW resources */
    glfwTerminate();
}

void Viewer::key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Viewer* viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
    viewer->on_key(key);
}

void Viewer::on_key(int key)
{
    // 'Q' or 'Escape' quits
    if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)
    {
        glfwSetWindowShouldClose(win, GLFW_TRUE);
    }
}

void Viewer::set_animation_fun(std::function<void()> fun){
    animation_fun = fun;
}