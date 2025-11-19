#include "Rbt.h"
#include <iostream>
using namespace std;

// Constructor
Rbt::Rbt() : root(nullptr){}

void Rbt::insert(int value){

    // Will create a new node with the value that is inserted
    RbtNode* newNode = new RbtNode(value);

    // Checks if the root is equal to null, then the root is the new node and will be set to black
    if (root == nullptr){
        root = newNode;
        newNode->setColor(RbtNode::CL_BLACK);
        return;
    }

    // Creates a node called current to be initially the root, but will be changed later when needed
    RbtNode* current = root;

    // Creates a parent node that is null, and will ten be changed later as things are inserted
    RbtNode* parent = nullptr;

    // Searches through the tree to check where the value should go
    while (current){
        parent = current;

        // Checks if the value is less than the valuethat the current node is, and will set the current node to be the left node based on the value
        if (value < current->getData()){
            current = current->getLeft();
        }
        
        // Will make the current node the right node based on the value
        else {
            current = current->getRight();
        }
    }

    // Sets the parent of the new node
    newNode->setParent(parent);

    // Checks if the value entered is less than the value of the parent, and then will set the left child of the parent to the new node
    if (value < parent->getData()){
        parent->setLeft(newNode);
    }
    
    // Will set the rigt child of the parent to the new node
    else {
        parent->setRight(newNode);
    }

    // Method call to the fix violations method
    fixViolations(newNode);
}

void Rbt::fixViolations(RbtNode* node){

    // Will always check for violations depending on the root, and and the parent's color is red
    while(node != root && node->getParent()->getColor() == RbtNode::CL_RED){
        
        // Creating a variable for the parent node
        RbtNode* parent = node->getParent();

        // Creating a variable for the grandparent node
        RbtNode* grandParent = parent->getParent();

        // checks the violations if the parent is on the left side of the tree
        if (parent == grandParent->getLeft()){

            // Creating an uncle variable
            RbtNode* uncle = grandParent->getRight();

            // Case 1
            if (uncle && uncle->getColor() == RbtNode::CL_RED){

                // Changing the color of the parent, uncle, and grandparent
                parent->setColor(RbtNode::CL_BLACK);
                uncle->setColor(RbtNode::CL_BLACK);
                grandParent->setColor(RbtNode::CL_RED);
                node = grandParent;
            } 
            
            // Case 2 and 4
            else {

                // Case 2 
                if (node == parent->getRight()){

                    // Rotates the parent to the left and  will then set the node to be te new parent, and update it
                    rotateLeft(parent);
                    node = parent;
                    parent = node->getParent();
                }

                // Case 4
                // Changes the color of the parent and the grandparent, and then puts the grandparent to the right
                parent->setColor(RbtNode::CL_BLACK);
                grandParent->setColor(RbtNode::CL_RED);
                rotateRight(grandParent);
            }
        }
        
        // Checks the violations if the parent is on the right side of the tree
        else {

            // Creates the uncle of the given node
            RbtNode* uncle = grandParent->getLeft();

            // Case 1
            if (uncle && uncle->getColor() == RbtNode::CL_RED){

                // Changing the color of the parent, uncle, and grandparent
                parent->setColor(RbtNode::CL_BLACK);
                uncle->setColor(RbtNode::CL_BLACK);
                grandParent->setColor(RbtNode::CL_RED);
                node = grandParent;
            }
            
            // Case 3 and 5
            else {

                // Case 3
                if (node == parent->getLeft()) {

                    // Rotates the parent to the right and  will then set the node to be te new parent, and update it
                    rotateRight(parent);
                    node = parent;
                    parent = node->getParent();
                }

                // Case 5
                // Changes the color of the parent and the grandparent, and then puts the grandparent to the left
                parent->setColor(RbtNode::CL_BLACK);
                grandParent->setColor(RbtNode::CL_RED);
                rotateLeft(grandParent);
            }
        }
    }

    // Will make sure that the root nodewill always be black
    root->setColor(RbtNode::CL_BLACK);
}

void Rbt::rotateLeft(RbtNode* node) {

    // Create a Right child so that it can become a new parent
    RbtNode* rightChild = node->getRight();

    //  Checks if a rotation can be done
    if (rightChild == nullptr) {
        return;
    }

    // Will update the parent of the right child to the original parent
    rightChild->setParent(node->getParent());

    // If the node is the root, update the root to be the right child
    if (node == root) {
        root = rightChild;
    } 
    
    // Updates the parents child pointer
    else if (node == node->getParent()->getLeft()){
        node->getParent()->setLeft(rightChild);
    } 
    
    // Updates the parents child pointer
    else {
        node->getParent()->setRight(rightChild);
    }

    // Sets the right child to what the left child was
    node->setRight(rightChild->getLeft());

    // Updates the parent pointer of the right child
    if (rightChild->getLeft() != nullptr) {
        rightChild->getLeft()->setParent(node);
    }

    // Make the original node the left child of he new parent
    rightChild->setLeft(node);

    // Update the parent of the original node
    node->setParent(rightChild);
}

void Rbt::rotateRight(RbtNode* node) {

    // Create a left child so that it can become a new parent
    RbtNode* leftChild = node->getLeft();

    // Checks if the rotation is possile
    if (leftChild == nullptr){
        return;
    }

    // Will pdate the parent of the left child to the original parent
    leftChild->setParent(node->getParent());

    // If the nod is the root, update the root to be the left child
    if (node == root){
        root = leftChild;
    }

    // Updates the parents child pointer
    else if (node == node->getParent()->getLeft()){
        node->getParent()->setLeft(leftChild);
    } 
    
    // Updates the paerent child pointer
    else {
        node->getParent()->setRight(leftChild);
    }

    // Sets the left child to hat th right child was
    node->setLeft(leftChild->getRight());

    // Updates the parentpointer of the left child
    if (leftChild->getRight() != nullptr){
        leftChild->getRight()->setParent(node);
    }

    // Make te original node  the right child of he new parent
    leftChild->setRight(node);

    // Update the parent of the original node
    node->setParent(leftChild);
}

bool Rbt::search(int value) const {

    // Create a current node and set it to root
    RbtNode* current = root;

    // Is used to determine where to go through the tree
    while (current){

        // Will return true if the value is found
        if (value == current->getData()){
            return true;
        }

        // Will set the current node to the left child if the value is less than the current data
        else if (value < current->getData()){
            current = current->getLeft();
        }

        // Will set the current node to the right child otherwise
        else {
            current = current->getRight();
        }
    }

    // Will return false otherwise
    return false;
}

int Rbt::min() const {

    // Will return -1 if the root is null, indicating an empty tree
    if (!root){
        return -1;
    }

    // Create a current node equal to root
    RbtNode* current = root;

    // Will go as far left as it can and then sets the current node to be the minimum value
    while (current->getLeft()){
        current = current->getLeft();
    }

    // Returns the minimum value
    return current->getData();
}

int Rbt::max() const {

    // Will return -1 if the root is null, indicating an empty tree
    if (!root){
        return -1;
    }

    // Create a current node equal to root
    RbtNode* current = root;

    // Will go as far right as it can and then sets the current node to be the maximum value
    while (current->getRight()){
        current = current->getRight();
    }

    // Returns the maximum value
    return current->getData();
}


int Rbt::size() const {

    // Recursive call to count the nodes
    return sizeRecursive(root);
}

// Recursive helper method to count the nodes
int Rbt::sizeRecursive(RbtNode* node) const {

    // Will return nothing if there is not a root
    if (node == nullptr){
        return 0;
    }

    // Counts the nodes on the left and right side
    int leftSize = sizeRecursive(node->getLeft());
    int rightSize = sizeRecursive(node->getRight());

    // Returns the total amount o nodes
    return 1 + leftSize + rightSize;
}

void Rbt::inorder() const {
    inorderTraversal(root);
    cout << endl;
}

void Rbt::inorderTraversal(RbtNode* node) const {

    // Checks if the node is not null
    if (node != nullptr) {

        // Will recursively call the function to get the node in order
        inorderTraversal(node->getLeft());

        // Will print out the value in order, and also the color associated to it
        cout << node->getData() << (node->getColor() == RbtNode::CL_RED ? "r " : "b ");

        // Will recursively call the function to get the node in order
        inorderTraversal(node->getRight());
    }
}