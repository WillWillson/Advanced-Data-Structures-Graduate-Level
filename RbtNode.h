#ifndef RBTNODEH
#define RBTNODEH

class RbtNode {
    private:

        // Number that is in the node
        int data;

        // Node color (0 for red, 1 for black)
        unsigned char color;

        // Pointer to the left and right child, and the parent
        RbtNode* left;
        RbtNode* right;
        RbtNode* parent;

    public:

        // Red Color
        static const unsigned char CL_RED = 0;   

        // Black Color
        static const unsigned char CL_BLACK = 1;

        // Constructor
        RbtNode(int value);

        // Accessors
        int getData() const;
        unsigned char getColor() const;
        RbtNode* getLeft() const;
        RbtNode* getRight() const;
        RbtNode* getParent() const;

        // Mutators
        void setData(int value);
        void setColor(unsigned char color);
        void setLeft(RbtNode* node);
        void setRight(RbtNode* node);
        void setParent(RbtNode* node);
    };

    #endif