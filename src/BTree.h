#pragma once

#include <vector>
#include <iostream>


class BTreeNode
{

private:

    int *keys;
    int n;
    int t;
    bool is_leaf;

public:

    BTreeNode **children;

    BTreeNode(int _t, bool _is_leaf)
    {
        n = 0;
        is_leaf = _is_leaf;
        t = _t;

        keys = new int[2 * _t - 1];
        children = new BTreeNode*[2 * _t];
    }

    inline bool is_full(void)
    {
        return n == 2 * t - 1;
    }

    // Print all keys in the node
    void print(void)
    {
        std::cout << "[";
        for (int i = 0; i < n; i++)
        {
            std::cout << keys[i];
            if (i < n - 1)
                std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }


    void print_from_node(std::string prefix = "", bool is_tail = true)
    {
        std::cout << prefix;
        std::cout << (is_tail ? "└── " : "├── ");

        print();
        
        // Print children
        if (!is_leaf)
        {
            for (int i = 0; i <= n; i++)
            {
                children[i]->print_from_node(prefix + (is_tail ? "    " : "│   "), i == n);
            }
        }
    }

    void split_child(int i)
    {
        BTreeNode *y = children[i];
        BTreeNode *z = new BTreeNode(t, y->is_leaf);

        // Add the new node to the x's children
        for (int j = n; j >= i + 1; j--)
        {
            children[j + 1] = children[j];
        }
        children[i + 1] = z;

        // Insert the median of the full child into x
        for (int j = n; j >= i; j--)
        {
            keys[j + 1] = keys[j];
        }
        keys[i] = y->keys[t - 1];
        n++;
        
        // Insert the last t - 1 keys of y into z
        for (int j = t; j < 2 * t - 1; j++)
        {
            z->keys[j - t] = y->keys[j];
        }

        if (!y->is_leaf)
        {
            for (int j = t; j < 2 * t; j++)
            {
                z->children[j - t] = y->children[j];
            }
        }

        z->n = y->n = t - 1;
    }

    void insert_non_full(int k)
    {
        int i = n - 1;
        if (is_leaf)
        {
            while (i >= 0 && k < keys[i])
            {
                keys[i + 1] = keys[i];
                i--;
            }
            keys[i + 1] = k;
            n++;
        }
        else
        {
            while (i >= 0 && k < keys[i])
            {
                i--;
            }
            i++;
            
            // Check if the CHILD is full
            if (children[i]->is_full())
            {
                split_child(i);
                // After split, determine which child to use
                if (k > keys[i])
                {
                    i++;
                }
            }
            children[i]->insert_non_full(k);
        }
    }

    BTreeNode *search(int k)
    {
        int i = 0;
        while (i < n && k > keys[i])
        {
            i++;
        }
        if (i < n && k == keys[i])
        {
            return this;
        }
        else if (is_leaf)
        {
            return nullptr;
        }
        else
        {
            return children[i]->search(k);
        }
    }
};


class BTree
{

private:

    BTreeNode *root;
    int t;

public:

    BTree(int _t)
    {
        root = new BTreeNode(_t, true);
        t = _t;
    }

    void insert(int k)
    {
        if (root->is_full()) // Root is full
        {
            BTreeNode *new_root = new BTreeNode(t, false);
            new_root->children[0] = root;
            root = new_root;
            root->split_child(0);
        }
        root->insert_non_full(k);
    }

    void print()
    {
        std::cout << "B-Tree (t=" << t << "):" << std::endl;
        root->print_from_node();
        std::cout << std::endl;
    }

    BTreeNode *search(int k)
    {
        return root->search(k);
    }
};