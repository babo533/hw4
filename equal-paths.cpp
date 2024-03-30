#ifndef RECCHECK
#include <iostream>
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int checkEqualPaths(Node *node) {
  if (!node) {
    return 0; // Reached null, return depth as 0
  } 
  // check for leaf
  if (!node->left && !node->right) { 
    return 1;
  }

  int leftDepth = 0, rightDepth = 0; // initialize depth

  // start recursing if child exists
  if (node->left) {
    leftDepth = checkEqualPaths(node->left);
  }
  if (node->right) {
    rightDepth = checkEqualPaths(node->right);
  }

  // change sides if one doesnt exist
  if (leftDepth == 0) {
    leftDepth = rightDepth;
  }

  if (rightDepth == 0) {
    rightDepth = leftDepth;
  }

  if (leftDepth == -1 || rightDepth == -1 || leftDepth != rightDepth) {
    return -1;
  }

  // return the incremented depth
  if (leftDepth >= rightDepth) {
    return leftDepth + 1;
  }
  else {
    return rightDepth + 1;
  }
  // Increment depth as we move up the tree
}

// Main function to determine if all paths from leaves to root have the same length
bool equalPaths(Node *node) {
    if (!node) {
      return true; // An empty tree or a single node tree has equal paths by definition
    } 
    return checkEqualPaths(node) != -1;
}
