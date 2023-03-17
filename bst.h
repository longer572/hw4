#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>

#include <algorithm>
#include <cmath>

/**
 * A templated class for a Node in a search tree.
 * The getters for parent/left/right are virtual so
 * that they can be overridden for future kinds of
 * search trees, such as Red Black trees, Splay trees,
 * and AVL trees.
 */
template <typename Key, typename Value>
class Node
{
public:
    Node(const Key& key, const Value& value, Node<Key, Value>* parent);
    virtual ~Node();

    const std::pair<const Key, Value>& getItem() const;
    std::pair<const Key, Value>& getItem();
    const Key& getKey() const;
    const Value& getValue() const;
    Value& getValue();

    virtual Node<Key, Value>* getParent() const;
    virtual Node<Key, Value>* getLeft() const;
    virtual Node<Key, Value>* getRight() const;

    void setParent(Node<Key, Value>* parent);
    void setLeft(Node<Key, Value>* left);
    void setRight(Node<Key, Value>* right);
    void setValue(const Value &value);

protected:
    std::pair<const Key, Value> item_;
    Node<Key, Value>* parent_;
    Node<Key, Value>* left_;
    Node<Key, Value>* right_;
};

/*
  -----------------------------------------
  Begin implementations for the Node class.
  -----------------------------------------
*/

/**
* Explicit constructor for a node.
*/
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent) :
    item_(key, value),
    parent_(parent),
    left_(NULL),
    right_(NULL)
{

}

/**
* Destructor, which does not need to do anything since the pointers inside of a node
* are only used as references to existing nodes. The nodes pointed to by parent/left/right
* are freed by the BinarySearchTree.
*/
template<typename Key, typename Value>
Node<Key, Value>::~Node()
{

}

/**
* A const getter for the item.
*/
template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const
{
    return item_;
}

/**
* A non-const getter for the item.
*/
template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem()
{
    return item_;
}

/**
* A const getter for the key.
*/
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const
{
    return item_.first;
}

/**
* A const getter for the value.
*/
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const
{
    return item_.second;
}

/**
* A non-const getter for the value.
*/
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue()
{
    return item_.second;
}

/**
* An implementation of the virtual function for retreiving the parent.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const
{
    return parent_;
}

/**
* An implementation of the virtual function for retreiving the left child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const
{
    return left_;
}

/**
* An implementation of the virtual function for retreiving the right child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const
{
    return right_;
}

/**
* A setter for setting the parent of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent)
{
    parent_ = parent;
}

/**
* A setter for setting the left child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left)
{
    left_ = left;
}

/**
* A setter for setting the right child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right)
{
    right_ = right;
}

/**
* A setter for the value of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value)
{
    item_.second = value;
}

/*
  ---------------------------------------
  End implementations for the Node class.
  ---------------------------------------
*/

/**
* A templated unbalanced binary search tree.
*/
template <typename Key, typename Value>
class BinarySearchTree
{
public:
    BinarySearchTree(); //TODO
    virtual ~BinarySearchTree(); //TODO
    virtual void insert(const std::pair<const Key, Value>& keyValuePair); //TODO
    virtual void remove(const Key& key); //TODO
    void clear(); //TODO
    bool isBalanced() const; //TODO
    void print() const;
    bool empty() const;

    template<typename PPKey, typename PPValue>
    friend void prettyPrintBST(BinarySearchTree<PPKey, PPValue> & tree);
public:
    /**
    * An internal iterator class for traversing the contents of the BST.
    */
    class iterator  // TODO
    {
    public:
        iterator();

        std::pair<const Key,Value>& operator*() const;
        std::pair<const Key,Value>* operator->() const;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;

        iterator& operator++();

    protected:
        friend class BinarySearchTree<Key, Value>;
        iterator(Node<Key,Value>* ptr);
        Node<Key, Value> *current_;
    };

public:
    iterator begin() const;
    iterator end() const;
    iterator find(const Key& key) const;
    Value& operator[](const Key& key);
    Value const & operator[](const Key& key) const;

protected:
    // Mandatory helper functions
    Node<Key, Value>* internalFind(const Key& k) const; // TODO
    Node<Key, Value> *getSmallestNode() const;  // TODO
    static Node<Key, Value>* predecessor(Node<Key, Value>* current); // TODO
    // Note:  static means these functions don't have a "this" pointer
    //        and instead just use the input argument.

    // Provided helper functions
    virtual void printRoot (Node<Key, Value> *r) const;
    virtual void nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2) ;

    // Add helper functions here

    // Finds successor
    static Node<Key, Value>* successor(Node<Key, Value>* current);

    // Helper function for finding successor
    static Node<Key, Value>* findMin(Node<Key, Value>* current);

    // Helper function for finding predecessor
    static Node<Key, Value>* findMax(Node<Key, Value>* current);

    // Helper function for clearing tree
    void clearHelper(Node<Key, Value>* current);

    // Helper function for inserting into tree
    void insertHelper(Node<Key, Value>* current, Node<Key, Value>* newNode);

    // Helper function for removal
    void removeHelper(Node<Key, Value>* current, const Key& key);

    // Helper function for internalFind
    Node<Key, Value>* findHelper(Node<Key, Value>* current, const Key& key) const;

    // Helper function for isBalanced
    int findHeight(Node<Key, Value>* current, int height) const;

    // Helper function for isBalanced
    bool balancedHelper(Node<Key, Value>* current, int length) const;

    // Helper function to promote a node
    void promoteNode(Node<Key, Value>* current, Node<Key, Value>* parent, Node<Key, Value>* child);

    // Helper function to delete a node
    // WARNING: MUST BE A LEAF NODE
    void removeNode(Node<Key, Value>* current);


protected:
    Node<Key, Value>* root_;
    // You should not need other data members
};

/*
--------------------------------------------------------------
Begin implementations for the BinarySearchTree::iterator class.
---------------------------------------------------------------
*/

/**
* Explicit constructor that initializes an iterator with a given node pointer.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value> *ptr)
{
    // TODO
    current_ = ptr;
}

/**
* A default constructor that initializes the iterator to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator() 
{
    // TODO
    current_ = NULL;
}

/**
* Provides access to the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> &
BinarySearchTree<Key, Value>::iterator::operator*() const
{
    return current_->getItem();
}

/**
* Provides access to the address of the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> *
BinarySearchTree<Key, Value>::iterator::operator->() const
{
    return &(current_->getItem());
}

/**
* Checks if 'this' iterator's internals have the same value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator==(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    // TODO
    if (rhs.current_ == NULL && this->current_ == NULL) return true;
    if (rhs.current_ != NULL && this->current_ == NULL) return false;
    if (rhs.current_ == NULL && this->current_ != NULL) return false;
    return ((*this)->first == rhs->first && (*this)->second == rhs->second);
}

/**
* Checks if 'this' iterator's internals have a different value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator!=(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    // TODO
    if (rhs.current_ == NULL && this->current_ == NULL) return false;
    if (rhs.current_ != NULL && this->current_ == NULL) return true;
    if (rhs.current_ == NULL && this->current_ != NULL) return true;
    return !((*this)->first == rhs->first && (*this)->second == rhs->second);

}


/**
* Advances the iterator's location using an in-order sequencing
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator&
BinarySearchTree<Key, Value>::iterator::operator++()
{
    // TODO
    this->current_ = successor(this->current_);
    return *this;

}


/*
-------------------------------------------------------------
End implementations for the BinarySearchTree::iterator class.
-------------------------------------------------------------
*/

/*
-----------------------------------------------------
Begin implementations for the BinarySearchTree class.
-----------------------------------------------------
*/

/**
* Default constructor for a BinarySearchTree, which sets the root to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree() 
{
    // TODO
    root_ = NULL;
}

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{
    // TODO
    clear();
}

/**
 * Returns true if tree is empty
*/
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const
{
    return root_ == NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{
    printRoot(root_);
    std::cout << "\n";
}

/**
* Returns an iterator to the "smallest" item in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::begin() const
{
    BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
    return begin;
}

/**
* Returns an iterator whose value means INVALID
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::end() const
{
    BinarySearchTree<Key, Value>::iterator end(NULL);
    return end;
}

/**
* Returns an iterator to the item with the given key, k
* or the end iterator if k does not exist in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::find(const Key & k) const
{
    Node<Key, Value> *curr = internalFind(k);
    BinarySearchTree<Key, Value>::iterator it(curr);
    return it;
}

/**
 * @precondition The key exists in the map
 * Returns the value associated with the key
 */
template<class Key, class Value>
Value& BinarySearchTree<Key, Value>::operator[](const Key& key)
{
    Node<Key, Value> *curr = internalFind(key);
    if(curr == NULL) throw std::out_of_range("Invalid key");
    return curr->getValue();
}
template<class Key, class Value>
Value const & BinarySearchTree<Key, Value>::operator[](const Key& key) const
{
    Node<Key, Value> *curr = internalFind(key);
    if(curr == NULL) throw std::out_of_range("Invalid key");
    return curr->getValue();
}

/**
* An insert method to insert into a Binary Search Tree.
* The tree will not remain balanced when inserting.
* Recall: If key is already in the tree, you should 
* overwrite the current value with the updated value.
*/
template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<const Key, Value> &keyValuePair)
{
    // TODO
    // Create new Node, then call helper function to insert into tree
    Node<Key, Value>* myNode = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, NULL);
    if (root_ == NULL) {
        root_ = myNode;
    } else {
        insertHelper(root_, myNode);
    }
    return;
}

template<class Key, class Value>
void BinarySearchTree<Key, Value>::insertHelper(Node<Key, Value>* current, Node<Key, Value>* newNode)
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
     *  Else, set newNode as left child of current
    */
    if (newNode->getKey() < current->getKey()) {
        if (current->getLeft() != NULL) {
            insertHelper(current->getLeft(), newNode);
            return;
        } else {
            newNode->setParent(current);
            current->setLeft(newNode);
            return;
        }
    }
    /**
     * Recursive case: newNode key is greater than current node's key
     * 
     * Desired action:
     *  If right child exists, recurse.
     *  Else, set newNode as right child of current
    */
    else if (newNode->getKey() > current->getKey()) {
        if (current->getRight() != NULL) {
            insertHelper(current->getRight(), newNode);
            return;
        } else {
            newNode->setParent(current);
            current->setRight(newNode);
        }
    }
}


/**
* A remove method to remove a specific key from a Binary Search Tree.
* Recall: The writeup specifies that if a node has 2 children you
* should swap with the predecessor and then remove.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key)
{
    // TODO
    removeHelper(root_, key);
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::removeHelper(Node<Key, Value>* current, const Key& key)
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
     *  If two children, swap with predecessor and remove
     *  If one child, promote child and remove
     *  If no children, remove
    */
    if (current->getKey() == key) {
        // No children case
        if (current->getRight() == NULL && current->getLeft() == NULL) {
            removeNode(current);
        }

        // Two children case

        else if (current->getRight() != NULL && current->getLeft() != NULL) {
            Node<Key, Value>* n = predecessor(current);
            nodeSwap(current, n);
            if (root_ == current) {
                root_ = n;
            }
            // Since current may still have two children, we recurse until
            // It either has one child or none, where it will be eventually removed
            removeHelper(current, key);
        }

        // One child case

        /**
         * WARNING: THIS CASE CAUSES BLOOD AND TEARS
        */
        else {
            // Determine which child exists, then promote child

            // If current has a left child     
            if (current->getLeft() != NULL) {

                // If root is to be removed
                if (root_ == current) {
                    promoteNode(current, NULL, current->getLeft());
                } else {
                    promoteNode(current, current->getParent(), current->getLeft());
                }
            // If current has a right child
            } else {
                // If root is to be removed
                if (root_ == current) {
                    promoteNode(current, NULL, current->getRight());
                } else {
                    promoteNode(current, current->getParent(), current->getRight());
                }
            }
        }
    }
    /**
     * Recursive case: Current node is not to be removed
     * 
     * Desired action: Recurse in the appropriate direction
    */
    else if (key > current->getKey()) {
        removeHelper(current->getRight(), key);
    } 
    else if (key < current->getKey()) {
        removeHelper(current->getLeft(), key);
    }
    return;
}



template<class Key, class Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::predecessor(Node<Key, Value>* current)
{
    // TODO
    if (current == NULL) return NULL;

    if (current->getLeft() != NULL) {
        return findMax(current->getLeft());
    }
    Node<Key, Value>* p = current->getParent();
    Node<Key, Value>* temp = current;
    if (p == NULL) return NULL;
    while (p->getRight() != temp && p != NULL) {
        temp = p;
        p = p->getParent();
        if (p == NULL) return NULL;
    }
    return p;
}

template<class Key, class Value>
Node<Key, Value>* BinarySearchTree<Key,Value>::successor(Node<Key, Value>* current)
{
    // TODO
    if (current == NULL) return NULL;

    if (current->getRight() != NULL) {
        return findMin(current->getRight());
    }
    Node<Key, Value>* p = current->getParent();
    Node<Key, Value>* temp = current;
    if (p == NULL) return NULL;
    while (p->getLeft() != temp && p != NULL) {
        temp = p;
        p = p->getParent();
        if (p == NULL) return NULL;
    }
    return p;

}

template<class Key, class Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::findMin(Node<Key, Value>* current)
{
    // TODO
    if (current == NULL) return NULL;
    if (current->getLeft() != NULL) {
        return findMin(current->getLeft());
    }
    return current;

}

template<class Key, class Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::findMax(Node<Key, Value>* current)
{
    // TODO
    if (current == NULL) return NULL;
    if (current->getRight() != NULL) {
        return findMax(current->getRight());
    }
    return current;

}

/**
* A method to remove all contents of the tree and
* reset the values in the tree for use again.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear()
{
    // TODO
    clearHelper(root_);

}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::promoteNode(Node<Key, Value>* current, Node<Key, Value>* parent, Node<Key, Value>* child)
{
    // If root is to be removed and there exists a, promote child
    if (parent == NULL) {

        // Move root_ pointer, set new root_'s parent to NULL, delete current
        root_ = child;
        child->setParent(NULL);
        delete current;

    } else {
        // Set current's child's parent to current's parent
        child->setParent(parent);

        // If current is a left child
        if (parent->getLeft() == current) {

            // Set child of current's parent to current's child
            parent->setLeft(child);

        // If current is a right child
        } else if (parent->getRight() == current) {

            // Set child of current's parent to current's child
            parent->setRight(child);
        }
        delete current;
    }
    return;
}

/**
 * WARNING: MUST BE A LEAF NODE
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::removeNode(Node<Key, Value>* current)
{
    // If current is null
    if (current == NULL) return;

    // If current is the root node
    if (current == root_) {
        root_ = NULL;
    // If current is a left child
    } else if (current->getParent()->getLeft() == current) {
        current->getParent()->setLeft(NULL);

    // If current is a right child
    } else if (current->getParent()->getRight() == current) {
        current->getParent()->setRight(NULL);

    }   
    current->setLeft(NULL);
    current->setRight(NULL);
    current->setParent(NULL);
    delete current;
    return;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clearHelper(Node<Key, Value>* current)
{
    if (current == NULL) return;
    clearHelper(current->getLeft());
    clearHelper(current->getRight());
    removeNode(current);
}


/**
* A helper function to find the smallest node in the tree.
*/
template<typename Key, typename Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::getSmallestNode() const
{
    // TODO
    return findMin(root_);
}

/**
* Helper function to find a node with given key, k and
* return a pointer to it or NULL if no item with that key
* exists
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const
{
    // TODO
    return findHelper(root_, key);
}
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::findHelper(Node<Key, Value>* current, const Key& key) const
{
    /**
     * Base case: current node is null
     * 
     * Desired action: return null
    */
    if (current == NULL) return NULL;

    /**
     * Base case: current node key matches key
     * 
     * Desired action: return current
    */
    if (current->getKey() == key) {
        return current;
    }
   /**
    * Recursive case: current node key ≠ key
    * 
    * Desired action: recurse in appropriate direction
   */
    else if (key > current->getKey()) {
        return findHelper(current->getRight(), key);
    }
    else {
        return findHelper(current->getLeft(), key);
    }
}

/**
 * Return true iff the BST is balanced.
 */
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const
{
    // TODO
    if (root_ == NULL) return true;

    return ((std::abs(findHeight(root_->getLeft(), 1) - findHeight(root_->getRight(), 1)) <= 1) && balancedHelper(root_->getLeft(), 1) && balancedHelper(root_->getRight(), 1));
}

template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::balancedHelper(Node<Key, Value>* current, int length) const
{
    if (current == NULL) return true;
    return (std::abs(findHeight(current->getLeft(), length + 1) - findHeight(current->getRight(), length + 1)) <= 1 && balancedHelper(current->getLeft(), length + 1) && balancedHelper(current->getRight(), length + 1));
}

template<typename Key, typename Value>
int BinarySearchTree<Key, Value>::findHeight(Node<Key, Value>* current, int height) const
{
    /**
     * Almost same implementation of checkPathLength in equal-paths
    */
    if (current == NULL) return 0;
    
    if (current->getRight() == NULL && current->getLeft() == NULL) return height;

    return std::max(findHeight(current->getLeft(), height+1), findHeight(current->getRight(), height+1));
}



template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2)
{
    if((n1 == n2) || (n1 == NULL) || (n2 == NULL) ) {
        return;
    }
    Node<Key, Value>* n1p = n1->getParent();
    Node<Key, Value>* n1r = n1->getRight();
    Node<Key, Value>* n1lt = n1->getLeft();
    bool n1isLeft = false;
    if(n1p != NULL && (n1 == n1p->getLeft())) n1isLeft = true;
    Node<Key, Value>* n2p = n2->getParent();
    Node<Key, Value>* n2r = n2->getRight();
    Node<Key, Value>* n2lt = n2->getLeft();
    bool n2isLeft = false;
    if(n2p != NULL && (n2 == n2p->getLeft())) n2isLeft = true;


    Node<Key, Value>* temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);

    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);

    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);

    if( (n1r != NULL && n1r == n2) ) {
        n2->setRight(n1);
        n1->setParent(n2);
    }
    else if( n2r != NULL && n2r == n1) {
        n1->setRight(n2);
        n2->setParent(n1);

    }
    else if( n1lt != NULL && n1lt == n2) {
        n2->setLeft(n1);
        n1->setParent(n2);

    }
    else if( n2lt != NULL && n2lt == n1) {
        n1->setLeft(n2);
        n2->setParent(n1);

    }


    if(n1p != NULL && n1p != n2) {
        if(n1isLeft) n1p->setLeft(n2);
        else n1p->setRight(n2);
    }
    if(n1r != NULL && n1r != n2) {
        n1r->setParent(n2);
    }
    if(n1lt != NULL && n1lt != n2) {
        n1lt->setParent(n2);
    }

    if(n2p != NULL && n2p != n1) {
        if(n2isLeft) n2p->setLeft(n1);
        else n2p->setRight(n1);
    }
    if(n2r != NULL && n2r != n1) {
        n2r->setParent(n1);
    }
    if(n2lt != NULL && n2lt != n1) {
        n2lt->setParent(n1);
    }


    if(this->root_ == n1) {
        this->root_ = n2;
    }
    else if(this->root_ == n2) {
        this->root_ = n1;
    }

}

/**
 * Lastly, we are providing you with a print function,
   BinarySearchTree::printRoot().
   Just call it with a node to start printing at, e.g:
   this->printRoot(this->root_) // or any other node pointer

   It will print up to 5 levels of the tree rooted at the passed node,
   in ASCII graphics format.
   We hope it will make debugging easier!
  */

// include print function (in its own file because it's fairly long)
#include "print_bst.h"

/*
---------------------------------------------------
End implementations for the BinarySearchTree class.
---------------------------------------------------
*/

#endif
