#include "Rbt.h"
#include <iostream>
using namespace std;

int main() {
    Rbt tree;

    // Insert values as specified
    tree.insert(50);
    tree.insert(25);
    tree.insert(75);
    tree.insert(12);
    tree.insert(37);
    tree.insert(62);
    tree.insert(87);
    tree.insert(45);
    tree.insert(20);
    tree.insert(30);
    tree.insert(60);
    tree.insert(80);
    tree.insert(70);
    tree.insert(10);
    tree.insert(90);

    // Search for a specific value
    int value1 = 20;
    int value2 = 27;

    // Using the searching function
    cout << "87 in tree? " << tree.search(87) << endl;
    cout << "90 in tree? " << tree.search(90) << endl;
    cout << "50 in tree? " << tree.search(50) << endl;
    cout << "21 in tree? " << tree.search(21) << endl;

     // Find the minimum and maximum values
    cout << "Min is " << tree.min() << endl;
    cout << "Max is " << tree.max() << endl;

    // Returns the size of the tree
    cout << "Nuumber of nodes: " << tree.size() << endl;

    // Use the inorder function
    tree.inorder();

    return 0;
}
