#include "RbtNode.h"

// Constructor: Initializes the node with a value
RbtNode::RbtNode(int value)
    : data(value), color(CL_RED), left(nullptr), right(nullptr), parent(nullptr) {}

// Accessors
int RbtNode::getData() const { 
    return data;
}

unsigned char RbtNode::getColor() const {
    return color;
}

RbtNode* RbtNode::getLeft() const {
    return left;
}

RbtNode* RbtNode::getRight() const {
    return right;
}

RbtNode* RbtNode::getParent() const {
    return parent;
}

// Mutators
void RbtNode::setData(int value){
    data = value;
}

void RbtNode::setColor(unsigned char color){
    this->color = color;
}

void RbtNode::setLeft(RbtNode* node){
    left = node;
}

void RbtNode::setRight(RbtNode* node){
    right = node;
}

void RbtNode::setParent(RbtNode* node){
    parent = node;
}