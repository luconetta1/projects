/*
 * BinarySearchTree.cpp
 * COMP15
 * Spring 2020
 *
 * Implementation of the Binary Search Tree class.
 * Behaves like a standard BST except that it handles multiplicity
 * If you insert a number twice  and then remove it once, then it will
 * still be in the data structure
 */

#include <cstring>
#include <iostream>
#include <limits>

#include "BinarySearchTree.h"

using namespace std;

BinarySearchTree::BinarySearchTree()
{
    root = nullptr;
}

BinarySearchTree::~BinarySearchTree()
{
        // walk tree in post-order traversal and delete
        post_order_delete(root);
        root = nullptr;
}

void BinarySearchTree::post_order_delete(Node *node)
{
    //if no nodes in tree
    if(node == nullptr){
        return;
    }

    if(node->left != nullptr){
        post_order_delete(node->left);
    }

    if(node->right != nullptr){
        post_order_delete(node->right);
    }

    delete node;
}

// copy constructor
BinarySearchTree::BinarySearchTree(const BinarySearchTree &source)
{
        // use pre-order traversal to copy the tree
        root = pre_order_copy(source.root);
}

// assignment overload
BinarySearchTree &BinarySearchTree::operator=(const BinarySearchTree &source)
{
        // TODO: Students write code here
        // check for self-assignment
        if (this != &source) {
            // delete current tree if it exists
            post_order_delete(root);
        }
        // use pre-order traversal to copy the tree
        root = pre_order_copy(source.root);
        // don't forget to "return *this"
        return *this;
}

BinarySearchTree::Node *BinarySearchTree::pre_order_copy(Node *node) const
{
        // TODO: Students write code here
        // (hint: use a pre-order traversal to copy details from the
        // node to a new node)
         if (node == nullptr) {
            return nullptr;
        }

        Node *new_node = new Node;
        new_node->data = node->data;
        new_node->count = node->count;
        new_node->left = pre_order_copy(node->left);
        new_node->right =  pre_order_copy(node->right);
        return new_node;
}

int BinarySearchTree::find_min() const
{
        if (root == nullptr) {
            return numeric_limits<int>::max(); // INT_MAX
        }

        return find_min(root)->data;
}

BinarySearchTree::Node *BinarySearchTree::find_min(Node *node) const
{
    Node *min = nullptr;

    //if tree is empty
    if (node == nullptr) {
        return nullptr;
    }

    //search until reaches last left node
    if(node->left != nullptr){
        node = find_min(node->left);
    }

    min = node;
    return min;

}

int BinarySearchTree::find_max() const
{
    if(root == nullptr) {
        return numeric_limits<int>::min(); // INT_MAX
    }

    return find_max(root)->data;

}

BinarySearchTree::Node *BinarySearchTree::find_max(Node *node) const
{
    Node *max = nullptr;

    //if tree is empty
    if (node == nullptr) {
        return nullptr;
    }

    //search until reaches last left node
    if(node->right != nullptr){
        node = find_max(node->right);
    }

    max = node;
    return max;
}

bool BinarySearchTree::contains(int value) const
{
    if(root == nullptr) {
        return false;
    }

    return contains(root, value);
}

bool BinarySearchTree::contains(Node *node, int value) const
{
    if (node == nullptr) {
        return false;
    }
    //base case: if node is equal to value
    if(node->data == value){
        return true;
    }

    //recurses left if value less than node data
    if(value < node->data){
        return contains(node->left, value);
    }

    //recurses right if value greater than node data
    if(value > node->data) {
        return contains(node->right, value);
    }
    return false;
}

void BinarySearchTree::insert(int value)
{
        insert(root, nullptr, value);
}

void BinarySearchTree::insert(Node *node, Node *parent, int value)
{
        if (node == nullptr) {
            Node *new_node = new Node;
            new_node->data = value;
            new_node->count = 1;
            if (parent == nullptr) {
                root = new_node;
            }else {
                if (parent->data > value){
                    parent->left = new_node;
                }else{
                    parent->right = new_node;
                }
            }
        }else if (value > node->data) {
            parent = node;
            insert(node->right, parent, value);
        }else if (value < node->data) {
            parent = node;
            insert(node->left, parent, value);
        }else {
            node->count += 1;
        }
}

bool BinarySearchTree::remove(int value)
{
        // TODO:  Students pick one of the following calls
        //        depending on which remove function they wrote
        //
        return remove(root, nullptr, value);
        //
        // return remove(root, &root, value);
}

// TODO:  Students uncomment one of the following remove functions
//        and implement it.  You only have to do one, though if you
//        want to explore both, that's ok, but turn in the program
//        with one of them working!
//
bool BinarySearchTree::remove(Node *node, Node *parent, int value)
{
    if(node == nullptr) {
        return false;
    }
    if(!contains(value)) {
        return false;
    }

    if(node->data < value) {
       parent = node;
       remove (node->right, parent, value);
       return true;
   }
   if (node->data > value) {
       parent = node;
       remove(node->left, parent, value);
       return true;
   }
   if(node->count > 1) {
       node->count--;
       return true;
   }
   //if node is a leaf
    if(node->left == nullptr && node->right == nullptr) {
        if(parent == nullptr) {
            delete root;
            root = nullptr;
            return true;
        }
        Node *temp = parent;
        //checks is node is to left or right of parent
        if(temp->left == node) {
            delete node;
            temp->left = nullptr;
        }
        else if (temp->right == node) {
            delete node;
            temp->right = nullptr;
        }
        return true;
    }


    //if node has one child to right
    if (node->left == nullptr && node->right != nullptr) {
    	if (parent == nullptr) {
    	    root = node->right;
    	}
    	//checks is node is to left or right of parent
    	else if(parent->right == node) {
    		parent->right = node->right;
    	}
    	else if(parent->left == node) {
    	    parent->left = node->right;
    	}
    	delete node;
    	return true;
    }

    //if node has one child to left
    if(node->left != nullptr && node->right == nullptr) {
        if(parent == nullptr) {
            root = node->left;
        }
        //checks if node is to left or right of parent
        else if(parent->right == node) {
    		parent->right = node->left;
    	}
    	else if(parent->left == node) {
    	    parent->left = node->left;
    	}
        delete node;
        return true;
    }

    //if node has two children
    if(node->left != nullptr && node->right != nullptr) {
    	Node *temp = find_min(node->right);
    	parent = find_parent(node, temp);
    	node->data = temp->data;
    	node->count = temp->count;
        int i = node->count;
    	if (node->data > parent->data) {
            while (i > 0){            //make sure to remove all of node that is
    	           remove(node->left, node, node->data);  // being replaced
                   i--;
            }
        }
    	else {
            while (i > 0){
    	           remove(node->right, node, node->data);
                   i--;
            }
    	}

    	return true;
        }
        return false;



}

// bool BinarySearchTree::remove(Node *node, Node **ptr_to_ptr_to_me, int value)
// {
//         // TODO:  Students write code here
// }

int BinarySearchTree::tree_height() const
{
        return tree_height(root);
}

int BinarySearchTree::tree_height(Node *node) const
{
    //if no tree exists
    if(node == nullptr) {
        return -1;
    }
     //if only node in tree
    if(node->left == nullptr && node->right == nullptr){
        return 0;
    }

    //search tree for longest node
    else {
        int left_height = tree_height(node->left);
        int right_height = tree_height(node->right);
        if(left_height > right_height) {
            return (left_height +1);
        }else {
            return (right_height +1);
        }
    }

}

// returns the total number of nodes
int BinarySearchTree::node_count() const
{
        return node_count(root);
}

int BinarySearchTree::node_count(Node *node) const
{
    if (node == nullptr){
        return 0;
    }

    //walks through tree, adds one for current node
    else {
        return (node_count(node->left) + node_count(node->right) + 1);
    }
}

// return the sum of all the node values (including duplicates)
int BinarySearchTree::count_total() const
{
        return count_total(root);
}

int BinarySearchTree::count_total(Node *node) const
{
    //if no nodes in tree
    if (node == nullptr){
        return 0;
    }

    //walks through tree adds data times the # times data appears in tree
    return (count_total(node->left) + count_total(node->right)
		+ node->data * node->count);

}

BinarySearchTree::Node *BinarySearchTree::find_parent(Node *node,
                                                      Node *child) const
{
        if (node == nullptr)
                return nullptr;

        // if either the left or right is equal to the child,
        // we have found the parent
        if (node->left == child or node->right == child) {
                return node;
        }

        // Use the binary search tree invariant to walk the tree
        if (child->data > node->data) {
                return find_parent(node->right, child);
        } else {
                return find_parent(node->left, child);
        }
}

// use the printPretty helper to make the tree look nice
void BinarySearchTree::print_tree() const
{
        size_t      numLayers  = tree_height() + 1;
        size_t      levelWidth = 4;
        const char *rootPrefix = "-> ";

        // Need numLayers * levelWidth characters for each layer of tree.
        // Add an extra levelWidth characters at front to avoid if statement
        // 1 extra char for nul character at end
        char *start = new char[(numLayers + 1) * levelWidth + 1];

        print_tree(root, start + levelWidth, start + levelWidth, rootPrefix);
        delete[] start;
}

// Logic and Output Reference:
// https://www.techiedelight.com/c-program-print-binary-tree/
void BinarySearchTree::print_tree(Node *node, char *const currPos,
                                  const char *const fullLine,
                                  const char *const branch) const
{
        if (node == nullptr)
                return;

        // 4 characters + 1 for nul terminator
        using TreeLevel                    = char[5];
        static const int       levelLength = sizeof(TreeLevel) - 1;
        static const TreeLevel UP = ".-- ", DOWN = "`-- ", EMPTY = "    ",
                               CONNECT = "   |";
        // Copies prev into dest and advances dest by strlen(prev)
        auto set = [](char *dest, const char *prev) {
                size_t p = strlen(prev);
                return (char *)memcpy(dest, prev, p) + p;
        };

        print_tree(node->right, set(currPos, EMPTY), fullLine, UP);

        // Clear any characters that would immediate precede the "branch"
        // Don't need to check for root (i.e start of array),fullLine is padded
        set(currPos - levelLength, EMPTY);

        // Terminate fullLine at current level
        *currPos = '\0';

        std::cerr << fullLine << branch << node->data
                  << (node->count > 1 ? "*" : "") << endl;

        // Connect upper branch to parent
        if (branch == UP)
                set(currPos - levelLength, CONNECT);

        // Connect lower branch to parent
        print_tree(node->left, set(currPos, CONNECT), fullLine, DOWN);
}
