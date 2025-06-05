#include "viewer.h"
#include "skybox.h"

#include <functional>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifndef SHADER_DIR
#error "SHADER_DIR not defined"
#endif

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX;
float lastY;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

Viewer::Viewer(int width, int height)
{
    this->width = width;
    this->height = height;

    lastX = width / 2.0f;
    lastY = height / 2.0f;

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

    // Set callback for camera handling
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
    glfwSetCursorPosCallback(win, mouse_callback);
    glfwSetScrollCallback(win, scroll_callback);
    
    // Hide the mouse and enable unlimited movement
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	// // Set the mouse at the center of the screen
	// glfwPollEvents();
	// glfwSetCursorPos(win, width/2.f, height/2.f);

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

    // get shader and textures directory
    shader_dir = SHADER_DIR;
    tex_dir = TEX_DIR;

    // initialize skybox
    skybox_shader = new Shader(shader_dir + "skybox.vert", shader_dir + "skybox.frag");
    std::vector<std::string> faces =
    {
    tex_dir + "left.png",
    tex_dir + "right.png",
    // tex_dir + "right.png",
    // tex_dir + "left.png",
    tex_dir + "top.png",
    tex_dir + "bottom.png",
    tex_dir + "front.png",
    tex_dir + "back.png"
    };
    skybox = new Skybox(skybox_shader, faces);
}



void Viewer::run()
{
    glm::mat4 id_mat = glm::mat4(1.f);

    // Main render loop for this OpenGL window
    while (!glfwWindowShouldClose(win))
    {
        // clear draw buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(win);

        float ratio = static_cast<float>(width)/height;
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), ratio, 0.1f, 100.0f);

        glm::mat4 view = camera.GetViewMatrix();

        // glm::mat4 scale = glm::scale(id_mat, glm::vec3(1.F, 1.F, 1.F));
        // glm::mat4 rotate = glm::mat4(1.f);
        // glm::mat4 translate = glm::translate(id_mat, glm::vec3(0.F, 0.F, 0.F));
        // glm::mat4 placement = translate * rotate;

        // glm::mat4 view = glm::lookAt(glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 1.f));
        // float ratio = static_cast<float>(width)/height;
        // glm::mat4 projection = glm::perspective(glm::radians(45.f), ratio, 0.1f, 20.0f);

        // Anime le modèle s'il a une fonction d'animation
        if(animation_fun) animation_fun();

        scene_root->draw(id_mat, id_mat, view, projection);

        skybox->draw(id_mat, view, projection);

        // Poll for and process events
        glfwPollEvents();

        // flush render commands, and swap draw buffers
        glfwSwapBuffers(win);
    }

    /* close GL context and any other GLFW resources */
    glfwTerminate();
    skybox->~Skybox();
    skybox_shader->~Shader();
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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}