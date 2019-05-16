// AVLSet.hpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// An AVLSet is an implementation of a Set that is an AVL tree, which uses
// the algorithms we discussed in lecture to maintain balance every time a
// new element is added to the set.  The balancing is actually optional,
// with a bool parameter able to be passed to the constructor to explicitly
// turn the balancing on or off (on is default).  If the balancing is off,
// the AVL tree acts like a binary search tree (e.g., it will become
// degenerate if elements are added in ascending order).
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to implement your AVL tree
// using your own dynamically-allocated nodes, with pointers connecting them,
// and with your own balancing algorithms used.

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include "Set.hpp"

#include <iostream>

template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    // Initializes an AVLSet to be empty, with or without balancing.
    explicit AVLSet(bool shouldBalance = true);

    // Cleans up the AVLSet so that it leaks no memory.
    virtual ~AVLSet() noexcept;

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s) noexcept;

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // height() returns the height of the AVL tree.  Note that, by definition,
    // the height of an empty tree is -1.
    int height() const;


    // preorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a preorder traversal of the AVL
    // tree.
    void preorder(VisitFunction visit) const;


    // inorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by an inorder traversal of the AVL
    // tree.
    void inorder(VisitFunction visit) const;


    // postorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a postorder traversal of the AVL
    // tree.
    void postorder(VisitFunction visit) const;

private:
    struct Node{
        ElementType value;
        Node* left;
        Node* right;
    };
    Node* head;

private:
    bool balance = false;
    int size_avl;
    void destroyAll(Node* head);
    void copyAll(Node* curr, Node* source);
    void addAll(Node*& curr, const ElementType& element);
    void preorderAll(VisitFunction visit, Node* curr) const;
    void inorderAll(VisitFunction visit, Node* curr) const;
    void postorderAll(VisitFunction visit, Node* curr) const;
    int heightAll(Node* curr) const;
    void rotateLeft(Node*& curr);
    void rotateLeftLeft(Node*& curr);
    void rotateRight(Node*& curr);
    void rotateRightRight(Node*& curr);


    // You'll no doubt want to add member variables and "helper" member
    // functions here.
};



// constructors

template <typename ElementType> // empty constructor, bool for balancing
AVLSet<ElementType>::AVLSet(bool shouldBalance)
{
    balance = shouldBalance;
    head = nullptr;
    size_avl = 0; 
}


template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
    destroyAll(head);
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
{
    head = nullptr;
    size_avl = s.size();
    copyAll(head, s.head);
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
{
    head = nullptr;
    size_avl = s.size();
    std::swap(head, s.head);
}

template <typename ElementType>
void AVLSet<ElementType>::destroyAll(Node* head)
{
    if(head != nullptr){
        if(head->left != nullptr){
            destroyAll(head->left);
        }
        if(head->right != nullptr){
            destroyAll(head->right);
        }
        delete head;

    }
}

template <typename ElementType>
void AVLSet<ElementType>::copyAll(Node* curr, Node* source)
{
    if (source == nullptr){
        curr = nullptr;
    }
    else{
        curr = new Node{source->value};
        if(curr->right == nullptr && curr->left == nullptr){
            return;
        }
        copyAll(curr->left,source->left);
        copyAll(curr->right,source->right);

    }
}


//functions

template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
    destroyAll(head);
    head = nullptr;
    copyAll(head,s.head);
    return *this;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
    std::swap(head, s.head);
    return *this;
}


template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{

    return true;
}

// balancing
template <typename ElementType>
void AVLSet<ElementType>::rotateLeft(Node*& curr){
    Node* n;
    n = curr->left;
    curr->left = n->right;
    n->right = curr;
    curr = n;
}

template <typename ElementType>
void AVLSet<ElementType>::rotateRight(Node*& curr){
    Node* n;
    n = curr->right;
    curr->right = n->left;
    n->left = curr;
    curr = n;

}

template <typename ElementType>
void AVLSet<ElementType>::rotateLeftLeft(Node*& curr){
    rotateRight(curr->left);
    rotateLeft(curr);
}

template <typename ElementType>
void AVLSet<ElementType>::rotateRightRight(Node*& curr){
    rotateLeft(curr->right);
    rotateRight(curr);
}


template <typename ElementType>
void AVLSet<ElementType>::addAll(Node*& curr, const ElementType& element)
{
    if(curr == nullptr){
        curr = new Node{element};
        size_avl++;
    }
    else if(element > curr->value){
        addAll(curr->right, element);
        if(balance && heightAll(curr->right) - heightAll(curr->left) == 2){
            if(element > curr->right->value){
                rotateRight(curr);
            }
            else{
                rotateRightRight(curr);
            }
        }
    }
    else if(element < curr->value){
        addAll(curr->left, element);
        if(balance && heightAll(curr->left) - heightAll(curr->right) == 2){
            if(element < curr->left->value){
                rotateLeft(curr);
            }
            else{
                rotateLeftLeft(curr);
            }
        }
    }

}

template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{
    addAll(head,element);
}


template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{
    Node* curr = head;
    while(curr != nullptr){
        if(element > curr->value){
            curr = curr->right;
        }
        else if(element < curr->value){
            curr = curr->left;
        }
        else if(element == curr->value){
            return true;
        }
    }
    return false;
}


template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    return size_avl;
}

template <typename ElementType>
int AVLSet<ElementType>::heightAll(Node* curr) const{
    if(curr == nullptr){
        return 0;
    }
    unsigned int right;
    unsigned int left;

    right = heightAll(curr->right);
    left = heightAll(curr->left);

    if(left>right){
        return 1 + left; 
    }
    else{
        return 1+ right;
    }

}

template <typename ElementType>
int AVLSet<ElementType>::height() const
{
    if (head == nullptr){
        return -1;
    }
    return heightAll(head)-1;
}


template <typename ElementType>
void AVLSet<ElementType>::preorderAll(VisitFunction visit, Node* curr) const
{
    if(curr != nullptr){
        visit(curr->value);
        preorderAll(visit,curr->left);
        preorderAll(visit,curr->right);

    }
}

template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
    preorderAll(visit, head);
}


template <typename ElementType>
void AVLSet<ElementType>::inorderAll(VisitFunction visit, Node* curr) const
{
    if(curr != nullptr){
        inorderAll(visit,curr->left);
        visit(curr->value);
        inorderAll(visit,curr->right);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
    inorderAll(visit,head);
}


template <typename ElementType>
void AVLSet<ElementType>::postorderAll(VisitFunction visit, Node* curr) const
{
    if(curr != nullptr){
        postorderAll(visit,curr->left);
        postorderAll(visit,curr->right);
        visit(curr->value);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
    postorderAll(visit,head);
}



#endif // AVLSET_HPP

