#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "shape.h"

class Shape;

class Node {
public:
    Node(const glm::mat4& transform = glm::mat4(1.0f));
    void add(Node* node);
    void add(Shape* shape);
    void draw(glm::mat4& placement, glm::mat4& scale, glm::mat4& view, glm::mat4& projection);
    void key_handler(int key) const;

    void set_Scale(glm::mat4 scale);
    void set_Rotate(glm::mat4 rotate);
    void set_Translate(glm::mat4 translate);

    void set_ScaleAnim(glm::mat4 scale);
    void set_RotateAnim(glm::mat4 rotate);
    void set_TranslateAnim(glm::mat4 translate);
    
private:
    glm::mat4 scale_;
    glm::mat4 rotate_;
    glm::mat4 translate_;
    glm::mat4 scale_anim_;
    glm::mat4 rotate_anim_;
    glm::mat4 translate_anim_;
    std::vector<Node *> children_;
    std::vector<Shape *> children_shape_;
};