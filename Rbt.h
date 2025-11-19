#ifndef RBTH
#define RBTH

#include "RbtNode.h"

class Rbt{
private:

    // Root node
    RbtNode* root;

    void fixViolations(RbtNode* node);
    void rotateLeft(RbtNode* node);
    void rotateRight(RbtNode* node);
    void inorderTraversal(RbtNode* node) const;
    int sizeRecursive(RbtNode* node) const;

public:
    Rbt();

    void insert(int value);
    bool search(int value) const;
    int min() const;
    int max() const;
    int size() const;
    void inorder() const;
};

#endif