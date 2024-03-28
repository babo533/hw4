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

    // Add helper functions here
    void adjustBalancesAndRotateAfterInsert(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* n);
    void rotateLeft(AVLNode<Key, Value>* n);
    
    AVLNode<Key, Value>* removeRecursive(AVLNode<Key, Value>* node, const Key& key);
    void removeFix(AVLNode<Key, Value>* node); // Declare removeFix
    void updateBalance(AVLNode<Key, Value>* node); // Declare updateBalance if needed
    void rebalance(AVLNode<Key, Value>* node); // Ensure rebalance is declared if used


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */


template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* y) {
    AVLNode<Key, Value>* x = y->getLeft();
    y->setLeft(x->getRight());

    if (x->getRight() != nullptr) {
        x->getRight()->setParent(y);
    }

    x->setParent(y->getParent());

    if (y->getParent() == nullptr) {
        this->root_ = x;
    } else if (y == y->getParent()->getLeft()) {
        y->getParent()->setLeft(x);
    } else {
        y->getParent()->setRight(x);
    }

    x->setRight(y);
    y->setParent(x);

    // Update balances with if statements
    if (x->getBalance() < 0) { // x was left-heavy
        y->setBalance(y->getBalance() + 1);
    } else { // x was balanced
        y->setBalance(y->getBalance() + 1 - x->getBalance());
    }
    
    if (y->getBalance() > 0) { // y became right-heavy
        x->setBalance(x->getBalance() + 1);
    } else { // y is balanced or left-heavy
        x->setBalance(x->getBalance() + 1 + y->getBalance());
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* x) {
    AVLNode<Key, Value>* y = x->getRight();
    x->setRight(y->getLeft());

    if (y->getLeft() != nullptr) {
        y->getLeft()->setParent(x);
    }

    y->setParent(x->getParent());

    if (x->getParent() == nullptr) {
        this->root_ = y;
    } else if (x == x->getParent()->getLeft()) {
        x->getParent()->setLeft(y);
    } else {
        x->getParent()->setRight(y);
    }

    y->setLeft(x);
    x->setParent(y);

    // Update balances with if statements
    if (y->getBalance() > 0) { // y was right-heavy
        x->setBalance(x->getBalance() - 1);
    } else { // y was balanced
        x->setBalance(x->getBalance() - 1 - y->getBalance());
    }
    
    if (x->getBalance() < 0) { // x became left-heavy
        y->setBalance(y->getBalance() - 1);
    } else { // x is balanced or right-heavy
        y->setBalance(y->getBalance() - 1 + x->getBalance());
    }
}



template<class Key, class Value>
void AVLTree<Key, Value>::rebalance(AVLNode<Key, Value>* node) {
    if (node->getBalance() > 1) {
        if (node->getLeft() != nullptr && node->getLeft()->getBalance() < 0) {
            // Left-Right case
            rotateLeft(node->getLeft());
        }
        // Left-Left case
        rotateRight(node);
    } else if (node->getBalance() < -1) {
        if (node->getRight() != nullptr && node->getRight()->getBalance() > 0) {
            // Right-Left case
            rotateRight(node->getRight());
        }
        // Right-Right case
        rotateLeft(node);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    // Start with checking if the tree is empty
    if (this->root_ == nullptr) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        // No need to adjust balances when the tree had been empty
        return;
    }

    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* newNode = nullptr;
    AVLNode<Key, Value>* parent = nullptr;

    // Find the correct place to insert the new node
    while (temp != nullptr) {
        parent = temp; // Keep track of the parent for later use
        if (new_item.first < temp->getKey()) {
            temp = static_cast<AVLNode<Key, Value>*>(temp->getLeft());
        } 
        else if (new_item.first > temp->getKey()) {
            temp = static_cast<AVLNode<Key, Value>*>(temp->getRight());
        } 
        else {
            // Key already exists, update the value
            temp->setValue(new_item.second);
            return; // No need to adjust balances for an update
        }
    }

    // Insert the new node
    if (new_item.first < parent->getKey()) {
        parent->setLeft(new AVLNode<Key, Value>(new_item.first, new_item.second, parent));
        newNode = static_cast<AVLNode<Key, Value>*>(parent->getLeft());
    } 
    else {
        parent->setRight(new AVLNode<Key, Value>(new_item.first, new_item.second, parent));
        newNode = static_cast<AVLNode<Key, Value>*>(parent->getRight());
    }

    // Adjust balances and rotate if necessary, starting from the parent of the new node
    adjustBalancesAndRotateAfterInsert(newNode);
}

template<class Key, class Value>
void AVLTree<Key, Value>::adjustBalancesAndRotateAfterInsert(AVLNode<Key, Value>* node) {
    // Begin from the newly inserted node, moving upwards to the root.
    while (node != nullptr) {
        AVLNode<Key, Value>* parent = node->getParent();

        // Stop if we've reached the root or if there's no imbalance to correct
        if (!parent) break;

        int balanceBefore = parent->getBalance();
        int balanceAfter = balanceBefore + (node == parent->getLeft() ? -1 : 1);
        parent->setBalance(balanceAfter);

        // If the balance factor after insertion is 0, the tree's height hasn't increased.
        if (balanceAfter == 0) break; // No further action needed up the tree.
        
        // If the balance factor's absolute value is 2, a rotation is required.
        if (abs(balanceAfter) == 2) {
            // Determine the type of rotation based on the balance factors.
            if (balanceAfter == -2) {
                if (node->getBalance() == 1) {
                    // Left-Right Case: Double rotation needed.
                    rotateLeft(node);
                }
                // Left-Left Case
                rotateRight(parent);
            } else if (balanceAfter == 2) {
                if (node->getBalance() == -1) {
                    // Right-Left Case: Double rotation needed.
                    rotateRight(node);
                }
                // Right-Right Case
                rotateLeft(parent);
            }
            
            // After rotation, the subtree is balanced; no need to go further up.
            break;
        }

        node = parent; // Move up to the next parent.
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key) {
    root_ = removeRecursive(root_, key);
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::removeRecursive(AVLNode<Key, Value>* node, const Key& key) {
    if (node == nullptr) {
        // Key not found, do nothing
        return nullptr;
    }

    if (key < node->getKey()) {
        node->setLeft(removeRecursive(node->getLeft(), key));
    } else if (key > node->getKey()) {
        node->setRight(removeRecursive(node->getRight(), key));
    } else {
        // Node with only one child or no child
        if ((node->getLeft() == nullptr) || (node->getRight() == nullptr)) {
            AVLNode<Key, Value> *temp = node->getLeft() ? node->getLeft() : node->getRight();

            // No child case
            if (temp == nullptr) {
                temp = node;
                node = nullptr;
            } else { // One child case
                *node = *temp; // Copy the contents of the non-empty child
            }
            delete temp;
        } else {
            // Node with two children: Get the inorder successor (smallest in the right subtree)
            AVLNode<Key, Value>* temp = minValueNode(node->getRight());

            // Copy the inorder successor's data to this node and delete the successor
            node->setKey(temp->getKey());
            node->setValue(temp->getValue());
            node->setRight(removeRecursive(node->getRight(), temp->getKey()));
        }
    }

    // If the tree had only one node then return
    if (node == nullptr) return nullptr;

    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    node->updateHeight();  // Assume we have a method to update height for AVLNode

    // STEP 3: REBALANCE THE NODE IF NEEDED
    return rebalance(node);
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
