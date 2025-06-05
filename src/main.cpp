#include "viewer.h"
#include "triangle.h"
#include "cylinder.h"
#include "node.h"
#include "shader.h"
#include "texture.h"
#include "textured_triangle.h"
#include "utils.h"
#include <string>
#include <GLFW/glfw3.h>

#ifndef SHADER_DIR
#error "SHADER_DIR not defined"
#endif

int main()
{
    // create window, add shaders & scene objects, then run rendering loop
    Viewer viewer(1080, 720);

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
    viewer.scene_root->add(main_node);

    // Test Triangle
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
    viewer.add_animation_fun(rotate_anim);

    // Human
    Node* human_anchor = new Node();
    human_anchor->set_Translate(glm::translate(id_mat, glm::vec3(3.f, 0.f, 0.f)));

    auto create_Node_cylinder = [color_shader](glm::mat4 model_mat = glm::mat4(1.F)) {
        Shape* shape = new Cylinder(color_shader);
        Node* node = new Node(model_mat);
        node->add(shape);
        return node;
    };

    Node* human = new Node();
    human_anchor->add(human);

    // Whole body
    glm::mat4 body_mat = simple_rotate(-90.f, "x");
    Node* body_node = new Node();
    body_node->set_Rotate(body_mat);
    human->add(body_node);

    // Upper and lower body
    Node* upper_body = new Node();
    Node* lower_body = new Node();
    lower_body->set_Translate(glm::translate(id_mat, glm::vec3(0.f, 0.f, -.75f)));
    body_node->add(upper_body);
    body_node->add(lower_body);

    // Main head node
    Node* head_node = create_Node_cylinder();
    head_node->set_Translate(glm::translate(id_mat, glm::vec3(.0f, .0f, 1.05f)));
    head_node->set_Scale(uniform_scale(0.55f));
    upper_body->add(head_node);

    // Neck
    Node* neck_node = create_Node_cylinder();
    neck_node->set_Scale(uniform_scale(0.6f));
    neck_node->set_Translate(glm::translate(id_mat, glm::vec3(.0f, .0f, -.4f)));
    head_node->add(neck_node);

    // Eyes
    glm::mat4 eye_scale = glm::scale(id_mat, glm::vec3(.3f, .2f, .35f));
    glm::mat4 eye_rotate = simple_rotate(90.f, "x");
    glm::vec3 left_eye_vec = glm::vec3(-.175f, -.175f, .15f);
    glm::vec3 right_eye_vec = left_eye_vec * glm::vec3(-1.F, 1.F, 1.F);

    Node* left_eye_node = create_Node_cylinder();
    left_eye_node->set_Rotate(eye_rotate);
    left_eye_node->set_Scale(eye_scale);
    left_eye_node->set_Translate(glm::translate(id_mat, left_eye_vec));
    head_node->add(left_eye_node);

    Node* right_eye_node = create_Node_cylinder();
    right_eye_node->set_Scale(eye_scale);
    right_eye_node->set_Rotate(eye_rotate);
    right_eye_node->set_Translate(glm::translate(id_mat, right_eye_vec));
    head_node->add(right_eye_node);

    // Mouth
    Shape* mouth = new Triangle(color_shader);
    Node* mouth_node = new Node();
    mouth_node->add(mouth);
    mouth_node->set_Scale(glm::scale(id_mat, glm::vec3(.6f, .15f, 1.f)));
    mouth_node->set_Rotate(simple_rotate(-90.f, "x"));
    mouth_node->set_Translate(glm::translate(id_mat, glm::vec3(.0f, -.275f, -.16f)));
    head_node->add(mouth_node);

    // Torso
    Node* torso_node = create_Node_cylinder();
    torso_node->set_Scale(glm::scale(id_mat, glm::vec3(.8f, .6f, 1.2f)));
    torso_node->set_Translate(glm::translate(id_mat, glm::vec3(0.f, 0.f, 0.f)));
    upper_body->add(torso_node);

    // Waist
    Node* waist_node = create_Node_cylinder();
    waist_node->set_Scale(glm::scale(id_mat, glm::vec3(.65f, .5f, .3f)));
    waist_node->set_Translate(glm::translate(id_mat, glm::vec3(0.f, 0.f, 0.f)));
    lower_body->add(waist_node);

    // Legs
    glm::vec3 invert_x = glm::vec3(-1.F, 1.F, 1.F);
    // Thights
    glm::mat4 thight_scale = glm::scale(id_mat, glm::vec3(.35F, .455F, 1.6f));  
    glm::vec3 left_thight_vec = glm::vec3(-0.15f, 0.f, -.38f);
    glm::vec3 right_thight_vec = left_thight_vec * invert_x;

    Node* left_thight_node = create_Node_cylinder();
    left_thight_node->set_Scale(thight_scale);
    left_thight_node->set_Translate(glm::translate(id_mat, left_thight_vec));
    waist_node->add(left_thight_node);

    Node* right_thight_node = create_Node_cylinder();
    right_thight_node->set_Scale(thight_scale);
    right_thight_node->set_Translate(glm::translate(id_mat, right_thight_vec));
    waist_node->add(right_thight_node);

    // Ankles
    glm::mat4 ankle_scale = glm::scale(id_mat, glm::vec3(.8F, .8F, 1.25F));
    glm::vec3 left_ankle_vec = glm::vec3(0.F, 0.F, -.5f);
    glm::vec3 right_ankle_vec = left_ankle_vec * invert_x;

    Node* left_ankle_node = create_Node_cylinder();
    left_ankle_node->set_Scale(ankle_scale);
    left_ankle_node->set_Translate(glm::translate(id_mat, left_ankle_vec));
    left_thight_node->add(left_ankle_node);

    Node* right_ankle_node = create_Node_cylinder();
    right_ankle_node->set_Scale(ankle_scale);
    right_ankle_node->set_Translate(glm::translate(id_mat,right_ankle_vec));
    right_thight_node->add(right_ankle_node);

    // Feet
    glm::mat4 feet_scale = glm::scale(id_mat, glm::vec3(1.F, 1.7F, 0.15F));
    glm::vec3 left_feet_vec = glm::vec3(.0f, -.05F, -.3F);
    glm::vec3 right_feet_vec = left_feet_vec * invert_x;

    Node* left_feet_node = create_Node_cylinder();
    left_feet_node->set_Scale(feet_scale);
    left_feet_node->set_Translate(glm::translate(id_mat, left_feet_vec));
    left_ankle_node->add(left_feet_node);

    Node* right_feet_node = create_Node_cylinder();
    right_feet_node->set_Scale(feet_scale);
    right_feet_node->set_Translate(glm::translate(id_mat, right_feet_vec));
    right_ankle_node->add(right_feet_node);
    
    // Arms
    // Shoulders
    glm::vec3 left_shoulder_vec = glm::vec3(-.5F, 0.F, .55F);
    glm::vec3 right_shoulder_vec = left_shoulder_vec * invert_x;

    Node* left_shoulder_node = new Node();
    left_shoulder_node->set_Translate(glm::translate(id_mat, left_shoulder_vec));
    torso_node->add(left_shoulder_node);

    Node* right_shoulder_node = new Node();
    right_shoulder_node->set_Translate(glm::translate(id_mat, right_shoulder_vec));
    torso_node->add(right_shoulder_node);

    // Biceps
    glm::mat4 biceps_scale = glm::scale(id_mat, glm::vec3(.75F * .4F, .4F, .5F));
    glm::vec3 left_biceps_translate_vec = glm::vec3(.0F, 0.F, -.4F);
    glm::vec3 right_biceps_translate_vec = left_biceps_translate_vec * invert_x;

    Node* left_biceps_node = create_Node_cylinder();
    left_biceps_node->set_Scale(biceps_scale);
    left_biceps_node->set_Translate(glm::translate(id_mat, left_biceps_translate_vec));
    left_shoulder_node->add(left_biceps_node);

    Node* right_biceps_node = create_Node_cylinder();
    right_biceps_node->set_Scale(biceps_scale);
    right_biceps_node->set_Translate(glm::translate(id_mat, right_biceps_translate_vec));
    right_shoulder_node->add(right_biceps_node);

    // Elbows
    glm::vec3 left_elbow_vec = glm::vec3(0.F, 0.F, -.225F);
    glm::vec3 right_elbow_vec = left_elbow_vec * invert_x;

    Node* left_elbow_node = new Node();
    left_elbow_node->set_Translate(glm::translate(id_mat, left_elbow_vec));
    left_biceps_node->add(left_elbow_node);

    Node* right_elbow_node = new Node();
    right_elbow_node->set_Translate(glm::translate(id_mat, right_elbow_vec));
    right_biceps_node->add(right_elbow_node);    

    // Forearms
    glm::mat4 forearm_scale = glm::scale(id_mat, glm::vec3(.85F, .85F, 1.F));
    glm::vec3 left_forearm_translate_vec = glm::vec3(0.F, 0.F, -.225F);
    glm::vec3 right_forearm_translate_vec = left_forearm_translate_vec * invert_x;

    Node* left_forearm_node = create_Node_cylinder();
    left_forearm_node->set_Scale(forearm_scale);
    left_forearm_node->set_Translate(glm::translate(id_mat, left_forearm_translate_vec));
    left_elbow_node->add(left_forearm_node);

    Node* right_forearm_node = create_Node_cylinder();
    right_forearm_node->set_Scale(forearm_scale);
    right_forearm_node->set_Translate(glm::translate(id_mat, right_forearm_translate_vec));
    right_elbow_node->add(right_forearm_node);

    // Hands
    glm::mat4 hand_scale = glm::scale(id_mat, glm::vec3(1.F, 1.25F, .1F));
    glm::mat4 hand_rotate = simple_rotate(90.F, "x");
    glm::vec3 hand_vec = glm::vec3(0.F, 0.F, -.35F);

    Node* left_hand_node = create_Node_cylinder();
    left_hand_node->set_Scale(hand_scale);
    left_hand_node->set_Rotate(hand_rotate);
    left_hand_node->set_Translate(glm::translate(id_mat, hand_vec));
    left_forearm_node->add(left_hand_node);

    Node* right_hand_node = create_Node_cylinder();
    right_hand_node->set_Scale(hand_scale);
    right_hand_node->set_Rotate(hand_rotate);
    right_hand_node->set_Translate(glm::translate(id_mat, hand_vec));
    right_forearm_node->add(right_hand_node);

    auto waving_animation = [&](){
        
        float time = static_cast<float>(glfwGetTime()) * 2.f;
        float init_angle = 110.f;
        float angle = glm::sin(time)*30.F + init_angle;
        float angle2 = glm::sin(time * 4)*15.F + 70.F;

        left_shoulder_node->set_RotateAnim(simple_rotate(angle, "y"));

        left_elbow_node->set_RotateAnim(simple_rotate(angle2, "y"));
    };
    viewer.add_animation_fun(waving_animation);
    // create_human_alex(human_anchor, color_shader, viewer);
    main_node->add(human_anchor);
    
    viewer.run();
}
