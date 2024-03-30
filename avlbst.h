#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    AVLNode<Key, Value>* rotateLeft(AVLNode<Key, Value>* x);
    AVLNode<Key, Value>* rotateRight(AVLNode<Key, Value>* y);


    void updateBalanceAndFix(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* insertHelper(AVLNode<Key, Value>* node, const std::pair<const Key, Value>& new_item);
    int height(AVLNode<Key, Value>* node) const;
    void updateBalance(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* balanceTree(AVLNode<Key, Value>* node);




};

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* x) {
  AVLNode<Key, Value>* y = x->getRight();
  x->setRight(y->getLeft());

  if (y->getLeft() != nullptr) {
    y->getLeft()->setParent(x);
  }

  y->setParent(x->getParent());

  if (x->getParent() == nullptr) {
    this->root_ = y;
  } 

  else if (x == x->getParent()->getLeft()) {
    x->getParent()->setLeft(y);
  } 

  else {
    x->getParent()->setRight(y);
  }
  y->setLeft(x);
  x->setParent(y);
  
  return y; // New root of the subtree
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* y) {
  if (!y) {
    return nullptr; // seg fault? 
  }

  AVLNode<Key, Value>* x = y->getLeft();

  if (!x) {
    return y; // cannot rotate without a left child
  }

  // Perform rotation
  y->setLeft(x->getRight());

  if (x->getRight() != nullptr) {
    x->getRight()->setParent(y);
  }
  
  x->setParent(y->getParent());
  
  if (y->getParent() == nullptr) {
    this->root_ = x;
  } 
  
  else if (y == y->getParent()->getLeft()) {
    y->getParent()->setLeft(x);
  } 
  
  else {
    y->getParent()->setRight(x);
  }
  
  x->setRight(y);
  y->setParent(x);

  return x; // Return the new root of the subtree
}


/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
  this->root_ = insertHelper(static_cast<AVLNode<Key, Value>*>(this->root_), new_item);
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::insertHelper(AVLNode<Key, Value>* node, const std::pair<const Key, Value>& new_item) {
  if (node == nullptr) {
    // new node if we've found the insert location
    return new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
  }

  if (new_item.first < node->getKey()) {
    node->setLeft(insertHelper(static_cast<AVLNode<Key, Value>*>(node->getLeft()), new_item));
    node->getLeft()->setParent(node);
  } 

  else if (new_item.first > node->getKey()) {
    node->setRight(insertHelper(static_cast<AVLNode<Key, Value>*>(node->getRight()), new_item));
    node->getRight()->setParent(node);
  } 

  else {
    // already exists, update value
    node->setValue(new_item.second);
    return node;
  }

  // update the balance factor of the node
  updateBalance(node);
  return balanceTree(node);
}

template<class Key, class Value> // asdjk
void AVLTree<Key, Value>::updateBalance(AVLNode<Key, Value>* node) { 
  if (node == nullptr) {
    return;
  }
  node->setBalance(height(node->getRight()) - height(node->getLeft()));
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::balanceTree(AVLNode<Key, Value>* node) {
  // left heavy
  if (node->getBalance() > 1) {
    if (node->getRight() != nullptr && node->getRight()->getBalance() < 0) {
      // RL 
      node->setRight(rotateRight(static_cast<AVLNode<Key, Value>*>(node->getRight())));
    }
    // RR 
    return rotateLeft(node);
  }
  //right heavy
  else if (node->getBalance() < -1) {
    if (node->getLeft() != nullptr && node->getLeft()->getBalance() > 0) {
      // LR 
      node->setLeft(rotateLeft(static_cast<AVLNode<Key, Value>*>(node->getLeft())));
    }
    // LL 
    return rotateRight(node);
  }

  return node;
}


template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key) {
  AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
  if (!node) {
    return; //  not found
  }

  // node with two children
  if (node->getLeft() && node->getRight()) {
    AVLNode<Key, Value>* predecessor = static_cast<AVLNode<Key, Value>*>(this->predecessor(node));
    nodeSwap(node, predecessor);
    // points to where 'predecessor' was and has at most one child
  }

  AVLNode<Key, Value>* child;
  if (node->getLeft()) {
    child = node->getLeft();
  } 
  else {
    child = node->getRight();
  }


  if (child) {
    child->setParent(node->getParent()); // link child to parent of the node to be removed
  }
  
  if (node->getParent()) {
    if (node == node->getParent()->getLeft()) {
      node->getParent()->setLeft(child);
    } 

    else {
      node->getParent()->setRight(child);
    }
    updateBalanceAndFix(node->getParent()); // rebalance from the parent of the removed node

  } 

  else {
    this->root_ = child; // child becomes the new root
  }

  delete node; // free the memory of the node to be removed
}

template<class Key, class Value>
void AVLTree<Key, Value>::updateBalanceAndFix(AVLNode<Key, Value>* node) {
  while (node) {
    int balance = height(node->getLeft()) - height(node->getRight());
    node->setBalance(balance);

    if (balance > 1) {
      if (height(node->getLeft()->getRight()) > height(node->getLeft()->getLeft())) {
          rotateLeft(node->getLeft());
      }
      rotateRight(node);
    } 
    
    else if (balance < -1) {
      if (height(node->getRight()->getLeft()) > height(node->getRight()->getRight())) {
          rotateRight(node->getRight());
      }
      rotateLeft(node);
    }

    node = node->getParent();  // Move up the tree
  }
}


template <class Key, class Value>
int AVLTree<Key, Value>::height(AVLNode<Key, Value>* node) const {
  if (node == nullptr) {
      // Height of an empty tree is -1
      return -1;
  } 

  else {
    // find the height of the left and right subtrees and add 1 for the current node
    int leftHeight = height(node->getLeft());
    int rightHeight = height(node->getRight());
    int max_height = 0;
    // Return the greater of the two heights plus one
    if (leftHeight > rightHeight) {
      max_height = leftHeight;
    } 
    else {
      max_height = rightHeight;
    }
    return max_height + 1;
  }
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif