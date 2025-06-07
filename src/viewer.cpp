#include "viewer.h"
#include "utils.h"

#include <functional>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifndef SHADER_DIR
#error "SHADER_DIR not defined"
#endif
#ifndef TEX_DIR
#error "TEX_DIR not defined"
#endif
#ifndef MODEL_DIR
#error "MODEL_DIR not defined"
#endif

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// camera
Camera camera(glm::vec3(0.0f, 1.7f, 10.0f));
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

    // get shaders, textures and models directory
    shader_dir = SHADER_DIR;
    tex_dir = TEX_DIR;
    model_dir = MODEL_DIR;

    // initialize skybox
    skybox_shader = new Shader(shader_dir + "skybox.vert", shader_dir + "skybox.frag");
    std::vector<std::string> faces = {
        tex_dir + "left.png",
        tex_dir + "right.png",
        tex_dir + "top.png",
        tex_dir + "bottom.png",
        tex_dir + "front.png",
        tex_dir + "back.png"
    };
    skybox = new Skybox(skybox_shader, faces);

    // initialize grass
    Texture* tex = new Texture(model_dir + "Grass.png");
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex->getGLid());

    // initialize ground
    Shader* texture_shader = new Shader(shader_dir + "ground.vert", shader_dir + "ground.frag");
    Texture* texture = new Texture(tex_dir + "texture_sol.jpg");
    ground = new Ground(texture_shader, texture, glm::vec3(-50.f, 10.f, -11.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.5f, 0.5f, 0.5f));
    ground_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f))
        * uniform_scale(200.f);
}


void Viewer::run()
{
    Shader* plane_shader = new Shader(shader_dir + "plane.vert", shader_dir + "plane.frag");

    Model grassModel(model_dir + "Low Grass.obj");

    Shader* model_shader = new Shader(shader_dir + "model.vert", shader_dir + "model.frag");

    Model planeModel(model_dir + "Light Aircraft.obj");

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    generateGrassGrid(25, 25, 5.0f);

    // Main render loop for this OpenGL window
    while (!glfwWindowShouldClose(win))
    {
        // clear draw buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(win);

        glm::mat4 view = camera.GetViewMatrix();
        float ratio = static_cast<float>(width) / height;
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), ratio, 0.1f, 1000.0f);

        // Draws the plane
        glUseProgram(plane_shader->get_id());

        GLint loc = glGetUniformLocation(plane_shader->get_id(), "projection");
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));

        loc = glGetUniformLocation(plane_shader->get_id(), "view");
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 model_plane = glm::mat4(1.0f);
        model_plane = glm::translate(model_plane, glm::vec3(-5.0f, -2.0f, -5.0f));
        model_plane = glm::scale(model_plane, glm::vec3(2.0f, 2.0f, 2.0f));

        loc = glGetUniformLocation(plane_shader->get_id(), "model");
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model_plane));

        loc = glGetUniformLocation(plane_shader->get_id(), "lightPos");
        glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(-50.f, 10.f, -11.f)));

        loc = glGetUniformLocation(plane_shader->get_id(), "viewPos");
        glUniform3fv(loc, 1, glm::value_ptr(camera.Position));

        loc = glGetUniformLocation(plane_shader->get_id(), "lightColor");
        glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

        loc = glGetUniformLocation(plane_shader->get_id(), "objectColor");
        glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));

        planeModel.Draw(*plane_shader);


        // Draws the grass
        glUseProgram(model_shader->get_id());
        
        loc = glGetUniformLocation(model_shader->get_id(), "projection");
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));

        loc = glGetUniformLocation(model_shader->get_id(), "view");
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view));

        loc = glGetUniformLocation(plane_shader->get_id(), "lightPos");
        glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(-50.f, 10.f, -11.f)));

        loc = glGetUniformLocation(plane_shader->get_id(), "viewPos");
        glUniform3fv(loc, 1, glm::value_ptr(camera.Position));

        loc = glGetUniformLocation(plane_shader->get_id(), "lightColor");
        glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

        loc = glGetUniformLocation(plane_shader->get_id(), "objectColor");
        glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));

        loc = glGetUniformLocation(plane_shader->get_id(), "model");

        for (const auto& model : model_matrices) {
            GLuint loc = glGetUniformLocation(plane_shader->get_id(), "model");
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));
            grassModel.Draw(*plane_shader);
        }

        // Draws the ground
	    ground->setCamera(camera.Position);

        ground->draw(ground_mat, view, projection);

        // Activate every animation function
        for(auto f : animation_funs) f();

        // Draws the character
        scene_root->draw(id_mat, id_mat, view, projection);

        // Draws the skybox
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
    ground->~Ground();
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

void Viewer::add_animation_fun(std::function<void()> fun){
    animation_funs.push_back(fun);
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

Camera Viewer::get_camera() {
        return camera;
}

// Generates the grass with a bit of jitter so it doesn't look too artificial
void Viewer::generateGrassGrid(int grid_x, int grid_z, float spacing) {
    model_matrices.clear();

    for (int x = -grid_x; x <= grid_x; ++x) {
        for (int z = -grid_z; z <= grid_z; ++z) {
            if (x <= 3.0f && x >= -3.0f && z <= 3.0f && z >= -3.0f) {
                continue;
            }
            else {
                float jitterX = ((rand() % 100) / 100.0f - 0.5f) * 2.0f;
                float jitterZ = ((rand() % 100) / 100.0f - 0.5f) * 2.0f;
                float scale = 40.0f + (rand() % 20);
                float angle = glm::radians((rand() % 360) * 1.0f);

                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(x * spacing + jitterX, -2.0f, z * spacing + jitterZ));
                model = glm::rotate(model, angle, glm::vec3(0, 1, 0));
                model = glm::scale(model, glm::vec3(scale));

                model_matrices.push_back(model);
            }
        }
    }
}