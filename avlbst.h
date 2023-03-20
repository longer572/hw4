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
    void leftRotate(AVLNode<Key, Value>* current, AVLNode<Key, Value>* rChild);
    void rightRotate(AVLNode<Key, Value>* current, AVLNode<Key, Value>* lChild);
    int getHeight(AVLNode<Key, Value>* current, int height=0);

    // When child and current are both right children (since it requires a left rotate)
    void zigZigLeftRotate(AVLNode<Key, Value>* current, AVLNode<Key, Value>* child);

    // When current is a right child and child is a left child 
    // Right rotate about current, left rotate about parent
    void zigZagLeftRotate(AVLNode<Key, Value>* current, AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child);

    // When child and current are both left children (since it requires a right rotate)
    void zigZigRightRotate(AVLNode<Key, Value>* current, AVLNode<Key, Value>* child);

    // When current is a left child and child is a right child 
    // Left rotate about current, right rotate about parent
    void zigZagRightRotate(AVLNode<Key, Value>* current, AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child);

    // Helper functions for insert
    void AVLinsertHelper(AVLNode<Key, Value>* current, AVLNode<Key, Value>* newNode);
    void insertFix(AVLNode<Key, Value>* current, AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child);

    // Helper functions for remove
    void AVLremoveHelper(AVLNode<Key, Value>* current, const Key& key);
    void removeFix(AVLNode<Key, Value>* current, int diff);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // First create a new node
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);

    // If empty tree, set newNode as root_ and return
    if (this->root_ == NULL) {
        this->root_ = newNode;
        return;
    }

    // Else call insert helper which is recursive (to avoid making new nodes on each call of newNode)
    AVLinsertHelper(static_cast<AVLNode<Key, Value>*>(this->root_), newNode);

}

template<class Key, class Value>
void AVLTree<Key, Value>::AVLinsertHelper(AVLNode<Key, Value>* current, AVLNode<Key, Value>* newNode)
{
    /**
     * Base case: key is found in tree
     * 
     * Desired action: Replace current value with new value
    */
    if (newNode->getKey() == current->getKey()) {
        current->setValue(newNode->getValue());
        delete newNode;
        return;
    }

    /**
     * Recursive case: newNode key is less than current node's key
     * 
     * Desired action:
     *  If left child exists, recurse.
     *  Else, set newNode as left child of current and update balance
    */
    if (newNode->getKey() < current->getKey()) {

        if (current->getLeft() != NULL) {
            AVLinsertHelper(current->getLeft(), newNode);
            return;

        } else {

            newNode->setParent(current);
            current->setLeft(newNode);

            if (current->getBalance() == 1) {
                current->setBalance(0);

            } else if (current->getBalance() == 0) {
                current->setBalance(-1);
                insertFix(current, current->getParent(), newNode);

            }
            return;
        }
    }
    /**
     * Recursive case: newNode key is greater than current node's key
     * 
     * Desired action:
     *  If right child exists, recurse.
     *  Else, set newNode as right child of current and update balance
    */
    else if (newNode->getKey() > current->getKey()) {

        if (current->getRight() != NULL) {
            AVLinsertHelper(current->getRight(), newNode);
            return;

        } else {

            newNode->setParent(current);
            current->setRight(newNode);

            if (current->getBalance() == -1) {
                current->setBalance(0);

            } else if (current->getBalance() == 0) {
                current->setBalance(1);
                insertFix(current, current->getParent(), newNode);
            }
        }
    }
}

template <class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* current, AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child)
{
    if (current == NULL || parent == NULL) return;

    /**
     * Left child case: Current is the left child of parent
    */
    if (parent->getLeft() == current) {

        parent->updateBalance(-1);

        // Case 1: balance of parent is now 0
        if (parent->getBalance() == 0) return;

        // Case 2: balance of parent is now -1
        if (parent->getBalance() == -1) {
            insertFix(parent, parent->getParent(), current);
            return;
        }

        // Case 3: balance of parent is now -2
        if (parent->getBalance() == -2) {

            // If zig-zig rotation required
            if (current->getLeft() == child) {

                zigZigRightRotate(parent, current);
                parent->setBalance(0);
                current->setBalance(0);

            // If zig-zag rotation required    
            } else {

                zigZagRightRotate(current, parent, child);

                // Case 3a: balance of child is -1
                if (child->getBalance() == -1) {

                    parent->setBalance(1);
                    current->setBalance(0);
                    child->setBalance(0);
                
                // Case 3b: balance of child is 0
                } else if (child->getBalance() == 0) {

                    parent->setBalance(0);
                    current->setBalance(0);
                    child->setBalance(0);
                    
                // Case 3c: balance of child is 1
                } else if (child->getBalance() == 1) {

                    parent->setBalance(0);
                    current->setBalance(-1);
                    child->setBalance(0);

                }
                
            }
            return;
        }
    }
    /**
     * Right child case: Current is the right child of parent
    */
    else if (parent->getRight() == current) {

        parent->updateBalance(1);

        // Case 1: balance of parent is now 0
        if (parent->getBalance() == 0) return;

        // Case 2: balance of parent is now 1
        if (parent->getBalance() == 1) {
            insertFix(parent, parent->getParent(), current);
            return;
        }

        // Case 3: balance of parent is now 2
        if (parent->getBalance() == 2) {

            // If zig-zig rotation required
            if (current->getRight() == child) {

                zigZigLeftRotate(parent, current);
                parent->setBalance(0);
                current->setBalance(0);

            // If zig-zag rotation required    
            } else {

                zigZagLeftRotate(current, parent, child);

                // Case 3a: balance of child is 1
                if (child->getBalance() == 1) {

                    parent->setBalance(-1);
                    current->setBalance(0);
                    child->setBalance(0);
                
                // Case 3b: balance of child is 0
                } else if (child->getBalance() == 0) {

                    parent->setBalance(0);
                    current->setBalance(0);
                    child->setBalance(0);
                    
                // Case 3c: balance of child is -1
                } else if (child->getBalance() == -1) {

                    parent->setBalance(0);
                    current->setBalance(1);
                    child->setBalance(0);

                }
                
            }
            return;
        }  
    }

}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    // TODO
    AVLremoveHelper(static_cast<AVLNode<Key, Value>*>(this->root_), key);
    return;
}

template<class Key, class Value>
void AVLTree<Key, Value>::AVLremoveHelper(AVLNode<Key, Value>* current, const Key& key)
{
    /**
     * Base case: current node is null
     * 
     * Desired action: return
    */
    if (current == NULL) return;

    /**
     * Base case: current node is to be removed
     * 
     * Desired action:
     *  If two children, swap with predecessor and remove, updating pointers
     *  If one child, promote child and remove
     *  If no children, remove
     *  
     *  Call removeFix to rebalance tree
    */
    if (current->getKey() == key) {

        int diff = 0;
        
        // Two children case
        if (current->getLeft() != NULL && current->getRight() != NULL) {

            // Swap current with predecessor
            nodeSwap(current, static_cast<AVLNode<Key, Value>*>(this->predecessor(current))); 
              
        }

        // Set diff
        AVLNode<Key, Value>* parent = current->getParent();
        if (parent != NULL) {

            // If current is a left child
            if (parent->getLeft() == current) {
                diff = 1;

            // If current is a right child
            } else if (parent->getRight() == current) {
                diff = -1;
            }
        }

        // If current has no children, remove
        if (current->getLeft() == NULL && current->getRight() == NULL) {
            this->removeNode(current);

        // If current has exactly one child, promote and remove
        // Note: current cannot have two children at this point
        } else if (current->getLeft() != NULL) {
            this->promoteNode(current, current->getParent(), current->getLeft());
        } else if (current->getRight() != NULL) {
            this->promoteNode(current, current->getParent(), current->getRight());
        }

        // Call removeFix (returns immediately if current has no parent)
        removeFix(parent, diff);
    }

    /**
     * Recursive case: Current node is not to be removed
     * 
     * Desired action: Recurse in the appropriate direction
    */
    else if (key > current->getKey()) {
        AVLremoveHelper(current->getRight(), key);
    } 
    else if (key < current->getKey()) {
        AVLremoveHelper(current->getLeft(), key);
    }
    return;
}

template<class Key, class Value>
void AVLTree<Key,Value>::removeFix(AVLNode<Key, Value>* current, int diff)
{
    // If current is null
    if (current == NULL) return;

    // Prepare next call's arguments before making changes
    AVLNode<Key, Value>* p = current->getParent();
    int ndiff = 0;
    if (p != NULL) {
        if (p->getLeft() == current) {
            ndiff = 1;
        } else {
            ndiff = -1;
        }
    }

    // Case where diff = -1
    if (diff == -1) {

        // Case 1: balance(current) + diff = -2
        if (current->getBalance() + diff == -2) {
            // Pointer to the tallest child (since diff is -1, it is the left child)
            AVLNode<Key, Value>* child = current->getLeft();

            // Case 1a: balance(current) = -1 (i.e. child has a left child)
            if (child->getBalance() == -1) {
                rightRotate(current, child);
                current->setBalance(0);
                child->setBalance(0);
                removeFix(p, ndiff);
            }

            // Case 1b: balance(current) = 0
            else if (child->getBalance() == 0) {
                rightRotate(current, child);
                current->setBalance(-1);
                child->setBalance(1);
                return;
            }

            // Case 1c: balance(current) = 1 (i.e. child has a right child)
            else if (child->getBalance() == 1) {
                AVLNode<Key, Value>* gChild = child->getRight();
                zigZagRightRotate(child, current, gChild);

                if (gChild->getBalance() == 1) {
                    current->setBalance(0);
                    child->setBalance(-1);
                    gChild->setBalance(0);

                } else if (gChild->getBalance() == 0) {
                    current->setBalance(0);
                    child->setBalance(0);
                    gChild->setBalance(0);
                     
                } else if (gChild->getBalance() == -1) {
                    current->setBalance(1);
                    child->setBalance(0);
                    gChild->setBalance(0);
                }
                removeFix(p, ndiff);
            }
        }

        // Case 2: balance(current) + diff = -1
        else if (current->getBalance() + diff == -1) {
            current->setBalance(-1);
            return;
        }

        // Case 3: balance(current) + diff = 0
        else if (current->getBalance() + diff == 0) {
            current->setBalance(0);
            removeFix(p, ndiff);
        }
    } else if (diff == 1) {

        // Case 1: balance(current) + diff = 2
        if (current->getBalance() + diff == 2) {
            // Pointer to the tallest child (since diff is 1, it is the right child)
            AVLNode<Key, Value>* child = current->getRight();

            // Case 1a: balance(current) = 1 (i.e. child has a right child)
            if (child->getBalance() == 1) {
                leftRotate(current, child);
                current->setBalance(0);
                child->setBalance(0);
                removeFix(p, ndiff);
            }

            // Case 1b: balance(current) = 0
            else if (child->getBalance() == 0) {
                leftRotate(current, child);
                current->setBalance(1);
                child->setBalance(-1);
                return;
            }

            // Case 1c: balance(current) = -1 (i.e. child has a left child)
            else if (child->getBalance() == -1) {
                AVLNode<Key, Value>* gChild = child->getLeft();
                zigZagLeftRotate(child, current, gChild);

                if (gChild->getBalance() == -1) {
                    current->setBalance(0);
                    child->setBalance(1);
                    gChild->setBalance(0);

                } else if (gChild->getBalance() == 0) {
                    current->setBalance(0);
                    child->setBalance(0);
                    gChild->setBalance(0);
                     
                } else if (gChild->getBalance() == 1) {
                    current->setBalance(-1);
                    child->setBalance(0);
                    gChild->setBalance(0);
                }
                removeFix(p, ndiff);
            }
        }

        // Case 2: balance(current) + diff = 1
        else if (current->getBalance() + diff == 1) {
            current->setBalance(1);
            return;
        }

        // Case 3: balance(current) + diff = 0
        else if (current->getBalance() + diff == 0) {
            current->setBalance(0);
            removeFix(p, ndiff);
        }
    }
    return;
}

template<class Key, class Value>
int AVLTree<Key, Value>::getHeight(AVLNode<Key, Value>* current, int height)
{
    if (current == NULL) return height;
    else return std::max(getHeight(current->getLeft(), height+1), getHeight(current->getRight(), height+1));
}

template<class Key, class Value>
void AVLTree<Key, Value>::leftRotate(AVLNode<Key, Value>* current, AVLNode<Key, Value>* rChild)
{
    // Handles movement of rChild's left child
    if (rChild->getLeft() != NULL) {
        current->setRight(rChild->getLeft());
        rChild->getLeft()->setParent(current);
    } else {
        current->setRight(NULL);
    }

    // Handles if current is root (i.e. has no parent)
    if (current->getParent() == NULL) {
        this->root_ = rChild;
        rChild->setParent(NULL);

    // If current is a left child   
    } else if (current->getParent()->getLeft() == current) {
        current->getParent()->setLeft(rChild);
        rChild->setParent(current->getParent());

    // If current is a right child
    } else if (current->getParent()->getRight() == current) {
        current->getParent()->setRight(rChild);
        rChild->setParent(current->getParent());
    }

    // Set rChild as parent of current, completing rotation
    current->setParent(rChild);
    rChild->setLeft(current);
    return;

}

template<class Key, class Value>
void AVLTree<Key, Value>::rightRotate(AVLNode<Key, Value>* current, AVLNode<Key, Value>* lChild)
{
    // Handles movement of lChild's right child
    if (lChild->getRight() != NULL) {
        current->setLeft(lChild->getRight());
        lChild->getRight()->setParent(current);
    } else {
        current->setLeft(NULL);
    }

    // Handles if current is root (i.e. has no parent)
    if (current->getParent() == NULL) {
        this->root_ = lChild;
        lChild->setParent(NULL);
    }

    // If current is a left child   
    else if (current->getParent()->getLeft() == current) {
        current->getParent()->setLeft(lChild);
        lChild->setParent(current->getParent());

    // If current is a right child
    } else if (current->getParent()->getRight() == current) {
        current->getParent()->setRight(lChild);
        lChild->setParent(current->getParent());
    }

    // Set lChild as parent of current, completing rotation
    current->setParent(lChild);
    lChild->setRight(current);
    return;
    // Adjust balances
    // current->setBalance(getHeight(current->getRight(), 0) - getHeight(current->getLeft(), 0));
    // lChild->setBalance(getHeight(lChild->getRight(), 0) - getHeight(lChild->getLeft(), 0));
}

// When child and current are both right children (since it requires a left rotate)
template <class Key, class Value>
void AVLTree<Key, Value>::zigZigLeftRotate(AVLNode<Key, Value>* current, AVLNode<Key, Value>* child)
{
    leftRotate(current, child);
    return;
}

// When current is a right child and child is a left child 
// Right rotate about current, left rotate about parent
template <class Key, class Value>
void AVLTree<Key, Value>::zigZagLeftRotate(AVLNode<Key, Value>* current, AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child)
{
    rightRotate(current, child);
    leftRotate(parent, child);
    return;
}

// When child and current are both left children (since it requires a right rotate)
template <class Key, class Value>
void AVLTree<Key, Value>::zigZigRightRotate(AVLNode<Key, Value>* current, AVLNode<Key, Value>* child)
{
    rightRotate(current, child);
    return;
}

// When current is a left child and child is a right child 
// Left rotate about current, right rotate about parent
template <class Key, class Value>
void AVLTree<Key, Value>::zigZagRightRotate(AVLNode<Key, Value>* current, AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child)
{
    leftRotate(current, child);
    rightRotate(parent, child);
    return;
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
