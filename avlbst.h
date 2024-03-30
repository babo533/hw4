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

   // void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    void rotateRight(AVLNode<Key, Value>* n);
    void rotateLeft(AVLNode<Key, Value>* n);
    //static int depth(AVLNode<Key, Value> * n);   //returns depth of node
    // AVLNode<Key, Value>* findMin(AVLNode<Key, Value>* node) const; // Declaration of findMin
    // AVLNode<Key, Value>* rebalance(AVLNode<Key, Value>* node);
    //AVLNode<Key, Value>* removeRecursive(AVLNode<Key, Value>* node, const Key& key);
    void adjustBalancesAndRotateAfterInsert(AVLNode<Key, Value>* node);
    //void replaceNodeInParent(AVLNode<Key, Value>* node, AVLNode<Key, Value>* child);
    void replaceNodeInParent(AVLNode<Key, Value>* node, AVLNode<Key, Value>* newNode);
    void updateBalanceAndFix(AVLNode<Key, Value>* node);
    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);
    int height(AVLNode<Key, Value>* node) const;




};

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* x) {
    AVLNode<Key, Value>* y = x->getRight();
    x->setRight(y->getLeft());
    if (y->getLeft() != nullptr) y->getLeft()->setParent(x);
    y->setParent(x->getParent());
    
    if (x->getParent() == nullptr) this->root_ = y;
    else if (x == x->getParent()->getLeft()) x->getParent()->setLeft(y);
    else x->getParent()->setRight(y);
    
    y->setLeft(x);
    x->setParent(y);

    // Update balance factors with explicit type cast
    x->setBalance(x->getBalance() + 1 - std::min(int8_t(0), y->getBalance()));
    y->setBalance(y->getBalance() + 1 + std::max(int8_t(0), x->getBalance()));
}



template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* y) {
    AVLNode<Key, Value>* x = y->getLeft();
    y->setLeft(x->getRight());
    if (x->getRight() != nullptr) x->getRight()->setParent(y);
    x->setParent(y->getParent());
    
    if (y->getParent() == nullptr) this->root_ = x;
    else if (y == y->getParent()->getRight()) y->getParent()->setRight(x);
    else y->getParent()->setLeft(x);
    
    x->setRight(y);
    y->setParent(x);

    // Update balance factors with explicit type cast
    y->setBalance(y->getBalance() - 1 - std::max(int8_t(0



/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    AVLNode<Key, Value>* temp = ((AVLNode<Key, Value>*)(this->root_));

    // TODO
    if (temp == NULL) {
      this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
      ((AVLNode<Key, Value>*)(this->root_))->setBalance(0);
      return;
    }
    else {
      //creates temporary Node pointer to traverse through BST
      while (temp != NULL) {
        //case for same key find
        if (temp->getKey() == new_item.first) {
          temp->setValue(new_item.second);
          return;
        }
        //go to left child
        if (new_item.first < temp->getKey()) {
          //if left child does not exist
          if (temp->getLeft() == NULL) {
            AVLNode<Key, Value>* toAdd = new AVLNode<Key, Value>(new_item.first, new_item.second, temp);
            temp->setLeft(toAdd);
            temp = temp->getLeft();
            break;
          }
          else {
            temp = temp->getLeft();
          }
        }
        //go to right child
        else if (new_item.first >= temp->getKey()) {
          //if right child does not exist
          if (temp->getRight() == NULL) {
            AVLNode<Key, Value>* toAdd = new AVLNode<Key, Value>(new_item.first, new_item.second, temp);
            temp->setRight(toAdd);
            temp = temp->getRight();
            break;
          }
          else {
            temp = temp->getRight();
          }
        }
      }
    }

    //end of inserting part

    temp->setBalance(0);

    //if the node that was added is not the root
    if (temp->getParent() != NULL) {
        if (temp->getParent()->getBalance() == -1) {
            temp->getParent()->setBalance(0);
        }
        else if (temp->getParent()->getBalance() == 1) {
            temp->getParent()->setBalance(0);
        }
        else if (temp->getParent()->getBalance() == 0) {
            if (temp == temp->getParent()->getLeft()) {
                temp->getParent()->setBalance(-1);
            }
            else {
                temp->getParent()->setBalance(1);
            }
            insertFix(temp->getParent(), temp);
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node) {
    while (parent) {
        int balance = parent->getBalance();
        int newBalance = (node == parent->getLeft()) ? --balance : ++balance;

        parent->setBalance(newBalance);

        // If the balance is zero after the update, the tree is now balanced
        if (parent->getBalance() == 0) break;

        if (abs(parent->getBalance()) > 1) {
            // Perform rotations
            if (parent->getBalance() < 0) {
                // Left heavy
                if (node->getBalance() > 0) {
                    // Left-Right Case
                    rotateLeft(node);
                }
                // Left-Left Case
                rotateRight(parent);
            } else {
                // Right heavy
                if (node->getBalance() < 0) {
                    // Right-Left Case
                    rotateRight(node);
                }
                // Right-Right Case
                rotateLeft(parent);
            }
            break; // After rotation, the tree is balanced
        }

        node = parent;
        parent = parent->getParent();
    }
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key) {
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (!node) return; // Node not found

    // Handling node with two children
    if (node->getLeft() && node->getRight()) {
        AVLNode<Key, Value>* predecessor = static_cast<AVLNode<Key, Value>*>(this->predecessor(node));
        nodeSwap(node, predecessor);
        // 'node' now points to where 'predecessor' was and has at most one child
    }

    // Now 'node' has at most one child
    AVLNode<Key, Value>* child = node->getLeft() ? node->getLeft() : node->getRight();

    if (child) {
        child->setParent(node->getParent()); // Link child to parent of the node to be removed
    }
    
    if (node->getParent()) {
        if (node == node->getParent()->getLeft()) {
            node->getParent()->setLeft(child);
        } else {
            node->getParent()->setRight(child);
        }
        updateBalanceAndFix(node->getParent()); // Start rebalancing from the parent of the removed node
    } else {
        this->root_ = child; // If node is root, the child becomes the new root
    }

    delete node; // Free the memory of the node to be removed
}

// Assuming updateBalanceAndFix is correctly implemented to adjust balances and perform rotations


template<class Key, class Value>
void AVLTree<Key, Value>::replaceNodeInParent(AVLNode<Key, Value>* node, AVLNode<Key, Value>* newNode) {
    if (node->getParent() == nullptr) {
        this->root_ = newNode; // newNode could be nullptr if node is a leaf
    } else {
        if (node == node->getParent()->getLeft()) {
            node->getParent()->setLeft(newNode);
        } else {
            node->getParent()->setRight(newNode);
        }
    }
    if (newNode != nullptr) {
        newNode->setParent(node->getParent());
    }
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
        } else if (balance < -1) {
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
        // Recursively find the height of the left and right subtrees and add 1 to account for the current node
        int leftHeight = height(node->getLeft());
        int rightHeight = height(node->getRight());

        // Return the greater of the two heights plus one
        return std::max(leftHeight, rightHeight) + 1;
    }
}

// Note: Implementation details of rotateLeft, rotateRight, nodeSwap, and getBalance methods are assumed to be existing.

// template<class Key, class Value>
// int8_t AVLTree<Key, Value>::calculateNewBalance(AVLNode<Key, Value>* node) {
//     int leftHeight = getBalance(node->getLeft());
//     int rightHeight = getBalance(node->getRight());
//     return static_cast<int8_t>(leftHeight - rightHeight);
// }



template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif