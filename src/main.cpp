#include "viewer.h"
#include "triangle.h"
#include "cylinder.h"
#include "node.h"
#include "shader.h"
#include "sphere.h"
#include "texture.h"
#include "textured_triangle.h"
#include <string>

#ifndef SHADER_DIR
#error "SHADER_DIR not defined"
#endif

int main()
{
    // create window, add shaders & scene objects, then run rendering loop
    Viewer viewer;

    // get shader directory
    std::string shader_dir = SHADER_DIR;

    Shader* texture_shader = new Shader(shader_dir + "texture.vert", shader_dir + "texture.frag");

    Texture* texture = new Texture("C:/Users/marco/OneDrive/Documents/TP OPENGL/TP3_material/textures/texture_sol2.png");
    Shape* ground = new TexturedTriangle(texture_shader, texture);
    glm::mat4 ground_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 100.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    Node* ground_node = new Node(ground_mat);

    ground_node->add(ground);

    viewer.scene_root->add(ground_node);

    Shader *color_shader = new Shader(shader_dir + "node.vert", shader_dir + "node.frag");

    // Trois matrice de base : celle du corps entier, et celles du haut et bas du corps.
    glm::mat4 human_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.2f, -6.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 upper_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 lower_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.1f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        
    Node* human = new Node(human_mat);
    Node* upper_body = new Node(upper_mat);
    Node* lower_body = new Node(lower_mat);
    human->add(upper_body);
    human->add(lower_body);

    // TODO create the human skeleton

    //Tête
    Shape* head = new Cylinder(color_shader);

    glm::mat4 head_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    Node* head_node = new Node(head_mat);
    head_node->add(head);
    human->add(head_node);

    //Cou
    Shape* neck = new Cylinder(color_shader);

    glm::mat4 neck_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.6f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.3f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    Node* neck_node = new Node(neck_mat);
    neck_node->add(neck);
    head_node->add(neck_node);

    //Yeux
    glm::mat4 eyes_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.45f, -0.1f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.15f, 0.15f, 0.15f));

    Node* eyes_node = new Node(eyes_mat);
    head_node->add(eyes_node);

    //Oeil droit
    Shape* right_eye = new Sphere(color_shader);
    glm::mat4 right_eye_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-1.3f, 0.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    Node* right_eye_node = new Node(right_eye_mat);
    right_eye_node->add(right_eye);
    eyes_node->add(right_eye_node);

    //Oeil gauche
    Shape* left_eye = new Sphere(color_shader);
    glm::mat4 left_eye_mat = glm::translate(glm::mat4(1.0f), glm::vec3(1.3f, 0.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    Node* left_eye_node = new Node(left_eye_mat);
    left_eye_node->add(left_eye);
    eyes_node->add(left_eye_node);
    
    //Bouche
    Shape* mouth = new Triangle(color_shader);

    glm::mat4 mouth_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.25f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    Node* mouth_node = new Node(mouth_mat);
    mouth_node->add(mouth);
    head_node->add(mouth_node);

    //Torse
    Shape* torso = new Cylinder(color_shader);

    glm::mat4 torso_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.05f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.9f, 1.2f, 0.7f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    Node* torso_node = new Node(torso_mat);
    torso_node->add(torso);
    upper_body->add(torso_node);
    
    //**************//
    
    //Bras
    //Bras droit
    glm::mat4 right_arm_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.7f, 0.1f, 0.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(-2.0f, 0.0f, 0.5f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.3f, 1.0f));

    Node* right_arm_node = new Node(right_arm_mat);
    upper_body->add(right_arm_node);

    //Haut du bras droit
    Shape* upper_right_arm = new Cylinder(color_shader);
    glm::mat4 upper_right_arm_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, -0.2f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.2f, 1.2f, 0.6f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    Node* upper_right_arm_node = new Node(upper_right_arm_mat);
    upper_right_arm_node->add(upper_right_arm);
    right_arm_node->add(upper_right_arm_node);

    //Avant bras droit
    Shape* lower_right_arm = new Cylinder(color_shader);
    glm::mat4 lower_right_arm_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.1f, 0.0f, 0.9f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.85f, 0.85f, 0.85f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        
    Node* lower_right_arm_node = new Node(lower_right_arm_mat);
    lower_right_arm_node->add(lower_right_arm);
    upper_right_arm_node->add(lower_right_arm_node);

    //Coude droit
    Shape* right_elbow = new Sphere(color_shader);
    glm::mat4 right_elbow_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.5f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 1.0f, 0.2f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    Node* right_elbow_node = new Node(right_elbow_mat);
    right_elbow_node->add(right_elbow);
    upper_right_arm_node->add(right_elbow_node);

    //Epaule droite
    Shape* right_shoulder = new Sphere(color_shader);
    glm::mat4 right_shoulder_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.6f, 0.0f, -0.5f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.1f, 1.1f, 0.2f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    Node* right_shoulder_node = new Node(right_shoulder_mat);
    right_shoulder_node->add(right_shoulder);
    right_arm_node->add(right_shoulder_node);

    //Main droite
    Shape* right_hand = new Sphere(color_shader);
    glm::mat4 right_hand_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.6f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.1f, 1.1f, 0.9f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    Node* right_hand_node = new Node(right_hand_mat);
    right_hand_node->add(right_hand);
    lower_right_arm_node->add(right_hand_node);

    //**************//
    
    //Bras gauche
    glm::mat4 left_arm_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.7f, 0.1f, 0.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(2.0f, 0.0f, 0.5f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.3f, 1.0f));

    Node* left_arm_node = new Node(left_arm_mat);
    upper_body->add(left_arm_node);

    //Haut du bras gauche
    Shape* upper_left_arm = new Cylinder(color_shader);
    glm::mat4 upper_left_arm_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, -0.2f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.2f, 1.2f, 0.6f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    Node* upper_left_arm_node = new Node(upper_left_arm_mat);
    upper_left_arm_node->add(upper_left_arm);
    left_arm_node->add(upper_left_arm_node);

    //Avant bras gauche
    Shape* lower_left_arm = new Cylinder(color_shader);
    glm::mat4 lower_left_arm_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.1f, 0.0f, 0.9f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.85f, 0.85f, 0.85f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    Node* lower_left_arm_node = new Node(lower_left_arm_mat);
    lower_left_arm_node->add(lower_left_arm);
    upper_left_arm_node->add(lower_left_arm_node);

    //Coude gauche
    Shape* left_elbow = new Sphere(color_shader);
    glm::mat4 left_elbow_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.5f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 1.0f, 0.2f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    Node* left_elbow_node = new Node(left_elbow_mat);
    left_elbow_node->add(left_elbow);
    upper_left_arm_node->add(left_elbow_node);

    //Epaule gauche
    Shape* left_shoulder = new Sphere(color_shader);
    glm::mat4 left_shoulder_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.6f, 0.0f, -0.5f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.1f, 1.1f, 0.2f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    Node* left_shoulder_node = new Node(left_shoulder_mat);
    left_shoulder_node->add(left_shoulder);
    left_arm_node->add(left_shoulder_node);

    //Main gauche
    Shape* left_hand = new Sphere(color_shader);
    glm::mat4 left_hand_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.6f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.1f, 1.1f, 0.9f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    Node* left_hand_node = new Node(left_hand_mat);
    left_hand_node->add(left_hand);
    lower_left_arm_node->add(left_hand_node);

    //**************//
    
    //Jambes
    //Jambe droite
    glm::mat4 right_leg_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.4f, 0.0f, 0.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(-3.0f, 0.0f, 0.5f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.4f, 1.0f));

    Node* right_leg_node = new Node(right_leg_mat);
    lower_body->add(right_leg_node);

    //Cuisse droite
    Shape* right_thigh = new Cylinder(color_shader);
    glm::mat4 right_thigh_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.25f, 0.0f, -0.15f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.3f, 1.3f, 0.6f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    Node* right_thigh_node = new Node(right_thigh_mat);
    right_thigh_node->add(right_thigh);
    right_leg_node->add(right_thigh_node);

    //Bas de la jambe droite
    Shape* lower_right_leg = new Cylinder(color_shader);
    glm::mat4 lower_right_leg_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.1f, 0.0f, 1.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 0.8f, 0.9f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    Node* lower_right_leg_node = new Node(lower_right_leg_mat);
    lower_right_leg_node->add(lower_right_leg);
    right_thigh_node->add(lower_right_leg_node);

    //Genou droit
    Shape* right_knee = new Sphere(color_shader);
    glm::mat4 right_knee_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.5f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.9f, 0.9f, 0.4f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    Node* right_knee_node = new Node(right_knee_mat);
    right_knee_node->add(right_knee);
    right_thigh_node->add(right_knee_node);

    //Fesse droite
    Shape* right_cheek = new Sphere(color_shader);
    glm::mat4 right_cheek_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.3f, -0.1f, -0.55f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.4f, 1.4f, 0.4f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    Node* right_cheek_node = new Node(right_cheek_mat);
    right_cheek_node->add(right_cheek);
    right_leg_node->add(right_cheek_node);

    //Pied droit
    Shape* right_foot = new Sphere(color_shader);
    glm::mat4 right_foot_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.1f, 0.2f, 0.6f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.1f, 1.5f, 0.4f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    Node* right_foot_node = new Node(right_foot_mat);
    right_foot_node->add(right_foot);
    lower_right_leg_node->add(right_foot_node);

    //**************//

    //Jambe gauche
    glm::mat4 left_leg_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.4f, 0.0f, 0.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(3.0f, 0.0f, 0.5f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.4f, 1.0f));

    Node* left_leg_node = new Node(left_leg_mat);
    lower_body->add(left_leg_node);

    //Cuisse gauche
    Shape* left_thigh = new Cylinder(color_shader);
    glm::mat4 left_thigh_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.25f, 0.0f, -0.15f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.3f, 1.3f, 0.6f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    Node* left_thigh_node = new Node(left_thigh_mat);
    left_thigh_node->add(left_thigh);
    left_leg_node->add(left_thigh_node);

    //Bas de la jambe gauche
    Shape* lower_left_leg = new Cylinder(color_shader);
    glm::mat4 lower_left_leg_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.1f, 0.0f, 1.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 0.8f, 0.9f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    Node* lower_left_leg_node = new Node(lower_left_leg_mat);
    lower_left_leg_node->add(lower_left_leg);
    left_thigh_node->add(lower_left_leg_node);

    //Genou gauche
    Shape* left_knee = new Sphere(color_shader);
    glm::mat4 left_knee_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.5f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.9f, 0.9f, 0.4f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    Node* left_knee_node = new Node(left_knee_mat);
    left_knee_node->add(left_knee);
    left_thigh_node->add(left_knee_node);

    //Fesse gauche
    Shape* left_cheek = new Sphere(color_shader);
    glm::mat4 left_cheek_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.3f, -0.1f, -0.55f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.4f, 1.4f, 0.4f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    Node* left_cheek_node = new Node(left_cheek_mat);
    left_cheek_node->add(left_cheek);
    left_leg_node->add(left_cheek_node);

    //Pied gauche
    Shape* left_foot = new Sphere(color_shader);
    glm::mat4 left_foot_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.1f, 0.2f, 0.6f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.1f, 1.5f, 0.4f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    Node* left_foot_node = new Node(left_foot_mat);
    left_foot_node->add(left_foot);
    lower_left_leg_node->add(left_foot_node);

    //**************//
    
    //Animation
    //On utilise ici une fonction qui ne renvoie rien et qui,
    //à l'aide de la méthode setTransform que j'ai définie dans node.h et node.cpp,
    //permet de mettre à jour la position des différentes parties du corps.
    auto animation = [&]() {
        //POur avoir des mouvements périodiques
        float t = glfwGetTime();
        float armAngle = sin(t) * -90.0f;
        float forearmAngle = (1+sin(t)) * 30.0f;

        //Mouvement des yeux
        eyes_node->setTransform(
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f + sin(t), 0.0f))
            * glm::scale(glm::mat4(1.0f), glm::vec3(2.0f + sin(t), 2.0f + sin(t), 2.0f + sin(t))));

        //Mouvement de balancement du bras droit
        right_arm_node->setTransform(
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))
            * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.55f, 0.0f))
            * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f))
            * glm::rotate(glm::mat4(1.0f), glm::radians(armAngle), glm::vec3(1.0f, 0.0f, 0.0f))
            * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.55f, 0.0f)));

        //Mouvement de l'avant bras droit
        lower_right_arm_node->setTransform(
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.5f))
            * glm::rotate(glm::mat4(1.0f), glm::radians(forearmAngle), glm::vec3(0.0f, 1.0f, -0.2f))
            * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f)));

        //Mouvement de balancement du bras gauche
        left_arm_node->setTransform(
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))
            * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.55f, 0.0f))
            * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f))
            * glm::rotate(glm::mat4(1.0f), glm::radians(armAngle), glm::vec3(1.0f, 0.0f, 0.0f))
            * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.55f, 0.0f)));

        //Mouvement de l'avant-bras droit
        lower_left_arm_node->setTransform(
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.5f))
            * glm::rotate(glm::mat4(1.0f), glm::radians(-forearmAngle), glm::vec3(0.0f, 1.0f, -0.2f))
            * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f)));

        //Mouvement de la bouche
        mouth_node->setTransform(
            glm::scale(glm::mat4(1.0f), glm::vec3(2.0f+sin(t), 2.0f+sin(t), 2.0f+sin(t))));
        };

    //On appelle setUpdateCallback qui est définie dans viewer.h sur la fonction animation,
    //pour la sauvegarder dans la fonction privée updateCallback dans viewer.h.
    viewer.setUpdateCallback(animation);
    viewer.scene_root->add(human);

    viewer.run();
}