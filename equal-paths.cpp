#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

// Returns the length of the leftmost path in the tree
int basePathLength(Node * root, int length);

// Function that compares each path length to the base path length
bool checkPathLength(Node * root, int length);


int basePathLength(Node * root, int length)
{

    /**
     * Base case: Root is null
     * 
     * Desired Action: Return 0
     */
    if (root == NULL) {
        return 0;
    }
    
    /**
     * Base case: Current node is a leaf node
     * 
     * Desired Action: Return length
     */
    if (root->left == NULL && root->right == NULL) {
        return length;
    }  

    /**
     * Recursive case: Proceed to left child if it exists, otherwise right child
     * 
     * Desired Action: Increment length and proceed to next nodes
    */
    if (root->left != NULL) {
        return basePathLength(root->left, length+1);
    } else {
        return basePathLength(root->right, length+1);
    }

}

bool checkPathLength(Node * root, int length, int baseLength)
{  
    /**
     * Base case: Node is null
     * 
     * Desired action: return true (X && 1 = X)
    */
    if (root == NULL) {
        return true;
    }
    /**
     * Base case: Node is a leaf
     * 
     * Desired action: return true if length = baseLength, false otherwise
    */
    if (root->left == NULL && root->right == NULL) {
        return (length == baseLength);
    }

    /**
     * Recursive case: Node has a child
     * 
     * Desired action: Increment length and move on
    */
   return (checkPathLength(root->left, length+1, baseLength) && checkPathLength(root->right, length+1, baseLength));

}


bool equalPaths(Node * root)
{
    // Add your code below
    if (!root) {
        return true;
    } else {
        int baseLength = basePathLength(root, 0);
        return checkPathLength(root, 0, baseLength);
    }
}

