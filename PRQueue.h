/**------------------------------------------
    Program 4: Priority Queue
    Description: 
    This program uses the concepts of classes, linked lsits, and Binary Search Trees in order to implement
    a fully-fleged priority queue data structure. It boasts a plethora of memmber functions in order to perform 
    various actions on the priority queue as well
    Year: Sophomore
    Course: CS 211, Fall 2023.
    System: VS Code on MacOS 14.1 Sonoma
    Author: Justin Vaughn, Adam T. Koehler
 ---------------------------------------------**/
/// @file prqueue.h
/// @author 
/// @date October 20, 2023
///
/// Assignment details and provided code are created and
/// owned by Adam T Koehler, PhD - Copyright 2023.
/// University of Illinois Chicago - CS 251 Fall 2023

#pragma once

#include <iostream>
#include <sstream>
#include <set>
#include <string>
using namespace std;

//priority queue class definitions with member functions
template<typename T>
class prqueue {
private:
    struct NODE {
        int priority;  // used to build BST
        T value;       // stored data for the p-queue
        bool dup;      // marked true when there are duplicate priorities
        NODE* parent;  // links back to parent
        NODE* link;    // links to linked list of NODES with duplicate priorities
        NODE* left;    // links to left child
        NODE* right;   // links to right child
    };
    NODE* root; // pointer to root NODE of the BST
    int sz;     // # of elements in the prqueue
    NODE* curr; // pointer to next item in prqueue (see begin and next)

/**---------------------------------------------------------------------------------------
 This private helper member function appends a NODE to the end of the linked list representing 
 duplicate priority values
---------------------------------------------------------------------------------------**/   
    void appendToLL(NODE* &headNODE, NODE* toAdd) {
        NODE* currNODE = headNODE;
        while (currNODE->link != NULL) {
            currNODE = currNODE->link;
        }
        currNODE->link = toAdd;
    }


/**---------------------------------------------------------------------------------------
 This private helper member function uses recursion to return a copy of a BST passed by 
 parameter via its root
---------------------------------------------------------------------------------------**/   
    NODE* helperForOperator(NODE* N) {
        //base case checking
        if (N == NULL) {
            return NULL;
        }
        NODE* copy = new NODE();
        copy->priority = N->priority;
        copy->value = N->value;
        copy->dup = N->dup;
        copy->parent = NULL;
        copy->link = NULL;
        //recursively calls function for both right and left subtress
        if (N->right != NULL) {
            copy->right = helperForOperator(N->right);
            if (copy->right != NULL) {
                copy->right->parent = copy;
            }
        }
        if (N->left != NULL) {
            copy->left = helperForOperator(N->left);
            if (copy->left != NULL) {
                copy->left->parent = copy;
            }
        }
        NODE* mainLink = N->link;
        NODE* copyLink = NULL;
        NODE* prev = NULL;
        while (mainLink != NULL) {
            NODE* newCopyLink = new NODE();
            newCopyLink->priority = mainLink->priority;
            newCopyLink->value = mainLink->value;
            newCopyLink->dup = mainLink->dup;
            newCopyLink->parent = NULL;
            newCopyLink->link = NULL;
            if (prev != NULL) {
                prev->link = newCopyLink;
            }
            else {
                copyLink = newCopyLink;
            }
            prev = newCopyLink;
            mainLink = mainLink->link;
        }
        copy->link = copyLink;
        return copy;
    }

/**---------------------------------------------------------------------------------------
 This private helper member function utlizes recursion in order to conver the current priority 
 queue into a string
---------------------------------------------------------------------------------------**/   
    void getNodesAsStr(NODE* node, stringstream& nodesString) const {
        if (node == NULL) {
            return;
        }
        //recursively calls function and adds to the stringstream object passed by reference
        getNodesAsStr(node->left, nodesString);
        nodesString << node->priority << " value: " << node->value << "\n";
        getNodesAsStr(node->link, nodesString);
        getNodesAsStr(node->right, nodesString);
        
    }

/**---------------------------------------------------------------------------------------
 This private helper member function utlizes recursion in order to delete all nodes in the 
 custom binary search tree
---------------------------------------------------------------------------------------**/ 
    void deleteTree(NODE* root) { 
        if (root == NULL) {
            return; 
        }
        deleteTree(root->left);
        deleteTree(root->link);  
        deleteTree(root->right);
        delete root; 
    }

/**---------------------------------------------------------------------------------------
 This private helper member finds the node whose value is the least in binary search tree 
 in order for use in the begin function
---------------------------------------------------------------------------------------**/ 
    NODE* findLeftMostNode(NODE* root) { 
        if (root->left == NULL) {
            return root; 
        }
        //recursive call to find leftmost node which is the least
        return findLeftMostNode(root->left);
    }  

public:
 
/**---------------------------------------------------------------------------------------
 This function serves as the default constructor for the prqueue class. 
 It intilizes the sz to zero as well the curr/data members to NULL
---------------------------------------------------------------------------------------**/
    prqueue() {
        sz = 0;
        curr = NULL;
        root = NULL;
    }
    
/**---------------------------------------------------------------------------------------
 This function creates a deep copy of a priority queue and also deletes the old one by 
 calling the previously defined clear function
 ---------------------------------------------------------------------------------------**/
    prqueue& operator=(const prqueue& other) {
        clear();
        //calls previously defined helper function
        this->root = helperForOperator(other.root);
        this->sz = other.sz;
        this->curr = other.curr;
        return (*this);
    }
    
/**---------------------------------------------------------------------------------------
 This member function frees all memory allocated on the heap, however it's public and 
 therefore can be utilized at any point
 ---------------------------------------------------------------------------------------**/
    void clear() {
        deleteTree(root);
        sz = 0;
        curr = NULL;
        root = NULL;
    }
    
/**---------------------------------------------------------------------------------------
 This function serves as a destructor for the prqueue class, it frees the memory allocated 
 on the heap
 ---------------------------------------------------------------------------------------**/
    ~prqueue() {
        clear();
    }
    
/**---------------------------------------------------------------------------------------
 This member function inserts the value into the custom BST in the correct location based on
 priority. It also determines if the value is a duplicate and appends it the end of the linked 
 list of duplicate if it is
 ---------------------------------------------------------------------------------------**/
    void enqueue(T value, int priority) {
        NODE* newEntry = new NODE();
        //sets appropiate values for a newEntry into the custom BST
        newEntry->parent = NULL;
        newEntry->left = NULL;
        newEntry->right = NULL;
        newEntry->link = NULL;
        newEntry->value = value;
        newEntry->priority =  priority;
        newEntry->dup = false;
        NODE* prevNODE = NULL;
        NODE* currNODE = root;
        while (currNODE != NULL) {
            if (priority == currNODE->priority) {
                newEntry->dup = true; //sets dup to true since value is a duplicate
                newEntry->parent = currNODE;
                //calls previously deinied helper function in order to append duplicate values to linked list
                appendToLL(currNODE, newEntry);
                sz++;
                return;
            }
            if (priority < currNODE->priority) {
                prevNODE = currNODE;
                currNODE  = currNODE->left;
            }
            else {
                prevNODE = currNODE;
                currNODE  = currNODE->right;
            }
        }
        if (prevNODE == NULL) {
            curr = NULL;
            root = newEntry;
        }
        else if (priority < prevNODE->priority) {
            prevNODE->left = newEntry;
            //sets parent pointers accordingly
            newEntry->parent = prevNODE;
        }
        else {
            prevNODE->right = newEntry;
            newEntry->parent = prevNODE;
        }
        sz++;
    }

/**---------------------------------------------------------------------------------------
 This member function returns the value of the next element in the priority queue and removes
 the specific element from the priority queue itself.
 ---------------------------------------------------------------------------------------**/
    T dequeue() {
        //edge checking
        if (root == NULL) {
            throw runtime_error("PQ is empty.");
        }
        NODE* curr = root;
        NODE* prev = NULL;

        while (curr->left != NULL) {
            prev = curr;
            curr = curr->left;
        }
        T valueOut = curr->value;

        if (curr->link != NULL) {
            //creates tempNODE to store curr data member being link
            NODE* tempNODE = curr->link;
            if (prev != NULL) {
                prev->left = tempNODE;
            }
            else {
                root = tempNODE;
            }
            tempNODE->left = curr->left;
            tempNODE->right = curr->right;
            tempNODE->parent = prev;
            delete curr;
        }

        else {
            if (prev != NULL) {
                prev->left = curr->right;
            }
            else {
                root = curr->right;
            }
            if (curr->right != NULL) {
                curr->right->parent = prev;
            }
            delete curr;
        }
        sz--;
        return valueOut;
    }

/**---------------------------------------------------------------------------------------
 This member function returns the size of the priority queue as an integer
 ---------------------------------------------------------------------------------------**/
    int size() {
        return sz; // TODO: update this return
    }
    
/**---------------------------------------------------------------------------------------
 This member function resets the internal state of the priority queue for an inorder traversal
 ---------------------------------------------------------------------------------------**/
    void begin() {
        curr = findLeftMostNode(root);
    }
        
/**---------------------------------------------------------------------------------------
 This member function ses the internal state to return the next inorder priority, and
 then advances the internal state in anticipation of future function calls. It also returns 
 a boolean varibles depending on wheter or not the internal state has reached NULL or not
 ---------------------------------------------------------------------------------------**/
    bool next(T& value, int &priority) {
        //edge checking
        value = curr->value;
        priority = curr->priority;

        if (curr == NULL) {
            return false;
        }
        
        if (curr->link != NULL) {
            curr = curr->link;
            return true;
        }

        //this block of code allows a node with a linked list of duplicates to get back to the orignal node to continue
        while ((curr->parent != NULL) && (curr->priority == curr->parent->priority)) {
            curr = curr->parent;
        }

        if (curr->right != NULL) {
            curr = curr->right;
            while (curr->left != NULL) {
                curr = curr->left;
            }
            return true;
        }
        else {
            while (curr->parent != NULL) {
                if (curr == curr->parent->left) {
                    curr = curr->parent;
                    return true;
                }
                curr = curr->parent;
            }

        if (curr->parent == NULL) {
            curr = NULL;
            return false;
        }
        return false;
    }
}
    
/**---------------------------------------------------------------------------------------
 This member function returns a string of the entire priority queue, in order by utilizing 
 a prior defined helper function. 
 ---------------------------------------------------------------------------------------**/
    string toString() {
        if (root == NULL) {
            return "";
        }
        stringstream nodesStr;
        getNodesAsStr(root, nodesStr);
        return nodesStr.str();
    }
    
/**---------------------------------------------------------------------------------------
 This member function operates in a similair fashion to the aforemntioned dequeue member function 
 except no deletion occurs, instead the value of the next priority queue element is returned
 with no extra modifications
 ---------------------------------------------------------------------------------------**/
    T peek() {
        if (root == NULL) {
            throw("PQ is empty.");
        }
        NODE* currNODE = root;
        while (currNODE->left != NULL) {
            currNODE = currNODE->left;
        }
        return currNODE->value;
    }
    
/**---------------------------------------------------------------------------------------
 This member function determines whether the created priority queue is equalivent to priority 
 queue passed in as a paremeter. It returns a boolean value
 ---------------------------------------------------------------------------------------**/
    bool operator==(const prqueue& other) const {
        stringstream STR1;
        getNodesAsStr(this->root, STR1);
        stringstream STR2;
        getNodesAsStr(other.root, STR2);
        //comapres the two stringstream objects after passing them to recurive helper function in order to define what it means when two prqueue are equal
        if (STR1.str() == STR2.str())
            return true;
        return false;
    }
    
/**---------------------------------------------------------------------------------------
 The member function returns the root of the BST
 ---------------------------------------------------------------------------------------**/
    void* getRoot() {
        return root;
    }
};
