#include "node.h"
#include "shape.h"
#include <iostream>
#include <GLFW/glfw3.h>

Node::Node(const glm::mat4& transform) {
    children_ = std::vector<Node*>();

    scale_ = glm::mat4(1.F);
    rotate_ = glm::mat4(1.F);
    translate_ = glm::mat4(1.F);

    scale_anim_ = glm::mat4(1.F);
    rotate_anim_ = glm::mat4(1.F);
    translate_anim_ = glm::mat4(1.F);
}

void Node::add(Node* node) {
    children_.push_back(node);
}

void Node::add(Shape* shape) {
    children_shape_.push_back(shape);
}

void Node::draw(glm::mat4& placement, glm::mat4& scale, glm::mat4& view, glm::mat4& projection) {
    // 'placement' is 'translate' * 'rotate' of the parent Node.

    glm::mat4 updatedPlacement = placement * translate_anim_ * translate_
        * rotate_anim_ * rotate_;

    glm::mat4 updatedScale = scale_anim_ * scale_ * scale;

    glm::mat4 updatedModel = updatedPlacement * updatedScale;

    for (auto child : children_shape_) {
        child->draw(updatedModel, view, projection);
    }

    for (auto child : children_) {
        child->draw(updatedPlacement, updatedScale, view, projection);
    }
}

void Node::key_handler(int key) const {
    for (const auto& child : children_) {
            child->key_handler(key);
    }
}

void Node::set_Scale(glm::mat4 scale){
    scale_ = scale;
}

void Node::set_Rotate(glm::mat4 rotate){
    rotate_ = rotate;
}

void Node::set_Translate(glm::mat4 translate){
    translate_ = translate;
}

void Node::set_ScaleAnim(glm::mat4 scale){
    scale_anim_ = scale;
}

void Node::set_RotateAnim(glm::mat4 rotate){
    rotate_anim_ = rotate;
}

void Node::set_TranslateAnim(glm::mat4 translate){
    translate_anim_ = translate;
}