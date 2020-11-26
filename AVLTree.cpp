#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
/*
NOTES:
How to structure the nested city Struct
Modify find successor to also search for potential successor in rightmost of left subtree
not just leftmost of of right subtree (not super necessary, but should do)

*/
class AVLTree
{
    struct City
    {
        string name, state;
        City(string name, string state)
        {
        }
        City* left = nullptr;
        City* right = nullptr;
    };
private:
    City* treeRoot = NULL;
    /*
    pass in both city name and state name because cities may not be uniquely identified by city name alone
    */
    City* insertion(City* root, string name, string state)
    {
        if (root == NULL)
        {
            City* temp = new City(name, state);
            root = temp;
            return root;
        }
        // duplicate check
        else if (root->name == name)
        {
            // duplicate check
            return root;
        }
        else if (root->name.compare(name) < 0)
        {
            if (root->left == nullptr)
            {
                City* temp = new City(name, state);
                root->left = temp;
                return root;
            }
            else
            {
                root->left = insertion(root->left, name, state);
                root = performRotation(root);
                return root;
            }
        }
        else if (root->name.compare(name) > 0)
        {
            if (root->right == nullptr)
            {
                City* temp = new City(name, state);
                root->right = temp;
                return root;
            }
            else
            {
                root->right = insertion(root->right, name, state);
                root = performRotation(root);
                return root;
            }
        }
        else
        {
            return root;
        }
    }
    City* search(City* root, string name, string state)
    {
        if (root == NULL)
        {
            return NULL;
        }
        else if (root->name == name)
        {
            return root;
        }
        else if (root->name.compare(name) < 0)
        {
            if (root->left != nullptr)
            {
                return search(root->left, name, state);
            }
            else
            {
                return NULL; // not found
            }
        }
        else if (root->name.compare(name) > 0)
        {
            if (root->right != nullptr)
            {
                return search(root->right, name, state);
            }
            else
            {
                return NULL; // not found
            }
        }
    }
    City* getSuccessorStudent(City* root)
    {
        if (root->left == nullptr)
        {
            return root;
        }
        else
        {
            return getSuccessorStudent(root->left);
        }
    }
    int treeHeight(City* root)
    {
        if (root == NULL)
            return 0;
        else
            return max(treeHeight(root->left), treeHeight(root->right)) + 1;
    }
    int getBalanceFactor(City* root)
    {
        return treeHeight(root->left) - treeHeight(root->right);
    }
    City* rotateLeft(City* node)
    {
        City* grandchild = node->right->left;
        City* newRoot = node->right;
        newRoot->left = node;
        node->right = grandchild;
        return newRoot;
    }
    City* rotateRight(City* node)
    {
        City* grandchild = node->left->right;
        City* newRoot = node->left;
        newRoot->right = node;
        node->left = grandchild;
        return newRoot;
    }
    City* rotateLeftRight(City* node)
    {
        City* newRoot = node->left->right;
        node->left = rotateLeft(node->left);
        rotateRight(node);
        return newRoot;
    }
    City* rotateRightLeft(City* node)
    {
        City* newRoot = node->right->left;
        node->right = rotateRight(node->right);
        rotateLeft(node);
        return newRoot;
    }
    City* performRotation(City* root)
    {
        if (getBalanceFactor(root) == 2 && getBalanceFactor(root->left) == 1)
        {
            return rotateRight(root);
        }
        else if (getBalanceFactor(root) == -2 && getBalanceFactor(root->right) == -1)
        {
            return rotateLeft(root);
        }
        else if (getBalanceFactor(root) == -2 && getBalanceFactor(root->right) == 1)
        {
            return rotateRightLeft(root);
        }
        else if (getBalanceFactor(root) == 2 && getBalanceFactor(root->left) == -1)
        {
            return rotateLeftRight(root);
        }
        else
        {
            return root; // no balancing needed
        }

    }
public:
    AVLTree()
    {
        treeRoot = NULL;
    }
};