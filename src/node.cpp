#include "node.h"
#include "shape.h"
#include <iostream>

Node::Node(const glm::mat4& transform) :
    transform_(transform) {

        children_ = std::vector<Node*>();
}

void Node::add(Node* node) {
    children_.push_back(node);
}

void Node::add(Shape* shape) {
    children_shape_.push_back(shape);
}

//Sauvegarde la transformation donnée en entrée et la garde en mémoire.
void Node::setTransform(glm::mat4& transform) {
    newtransform_ = transform;
}

void Node::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    //On ajoute ici newtransform_ pour rajouter la "mise à jour" de position de l'animation.
    glm::mat4 updatedModel = model * newtransform_ * transform_;

    for (auto child : children_) {
        child->draw(updatedModel, view, projection);
    }

    for (auto child : children_shape_) {
        child->draw(updatedModel, view, projection);
    }
}

void Node::key_handler(int key) const {
    for (const auto& child : children_) {
            child->key_handler(key);
    }
}
