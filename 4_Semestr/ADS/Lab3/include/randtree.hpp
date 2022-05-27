#pragma once

#include "bst.hpp"

#include <cstdlib>
#include <ctime>


template <typename Key>
class randtree : public bst<Key>
{
public:
    using value_type             = Key;
    using base                   = bst<value_type>;
    using node_type              = typename base::node_type;
    using iterator               = typename base::iterator;
    using size_type              = size_t;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using pointer                = value_type*;
    using const_pointer          = const value_type*;
    using const_iterator         = const iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
    randtree() : base()
    { std::srand(std::time(nullptr)); }

    iterator
    insert(const_reference value) override
    {
        if (base::_root != nullptr)
            return __insert(value, base::_root);

        base::_root  = new node_type(value, base::_base);
        base::_first = base::_root;
        base::_last  = base::_root;
        base::_size++;

        base::_base->_right = base::_first;
        base::_base->_left  = base::_last;

        return iterator(base::_root);
    }

    void
    erase(const_reference value) override
    { erase(base::find(value)); }

    void
    erase(iterator pos) override
    {
        node_type* node   = pos._node;
        node_type* parent = node->_parent;

        node_type* merged_node = merge(node->_left, node->_right);
        merged_node->_parent   = parent;

        (parent->_left == node
            ? parent->_left
            : parent->_right
        ) = merged_node;

        delete node;
        --base::_size;
    }

private:
    bool
    is_root_insertion(node_type* node)
    {
        float var  = 100.f / (get_weight(node) + 1);
        int random = std::rand() % 101;

        return random <= var;
    }

    node_type*
    get_merging_root(node_type* left, node_type* right)
    {
        int lw = get_weight(left);
        int rw = get_weight(right);

        return (lw > rw ? left : right);
    }

    node_type*
    merge(node_type* left, node_type* right)
    {
        if (left == nullptr)
            return right;

        if (right == nullptr)
            return left;

        node_type* merging_root = get_merging_root(left, right);
        node_type* new_parent;

        if (merging_root == left) {
            new_parent         = base::get_last(merging_root);
            new_parent->_right = right;
            right->_parent     = new_parent;
        } else {
            new_parent        = base::get_first(merging_root);
            new_parent->_left = left;
            left->_parent     = new_parent;
        }
        
        return merging_root;
    }

    int
    get_weight(node_type* node)
    {
        if (node == nullptr)
            return 0;

        return 1 + get_weight(node->_left) + get_weight(node->_right);
    }

    iterator
    __insert(const_reference value, node_type* node)
    {
        if (value == node->_value)
            return iterator(node);

        if (is_root_insertion(node)) {
            node_type* inserted = root_insert(value, node);

            if (node == base::_root)
                base::_root = inserted;

            return iterator(inserted);
        }

        // left insertion
        if (value < node->_value) {
            if (node->_left == nullptr)
                return base::raw_insert(node->_left, node, value);
            return __insert(value, node->_left);
        }

        // right insertion
        if (node->_right == nullptr)
            return base::raw_insert(node->_right, node, value);
        return __insert(value, node->_right);
    }

    node_type*
    root_insert(const_reference value, node_type* node)
    {
        if (value == node->_value)
            return node;

        node_type* new_root;

        // left insertion
        if (value < node->_value) {
            if (node->_left == nullptr)
                new_root = base::raw_insert(node->_left, node, value)._node;
            else
                new_root = root_insert(value, node->_left);

            rotate_right(node, new_root);
        }
        // right insertion
        else {
            if (node->_right == nullptr)
                new_root = base::raw_insert(node->_right, node, value)._node;
            else
                new_root = root_insert(value, node->_right);

            rotate_left(node, new_root);
        }

        return new_root;
    }

    void
    rotate_left(node_type* cur_root, node_type* new_root)
    {
        node_type* cur_parent = cur_root->_parent;
        new_root->_parent     = cur_parent;

        node_type* new_parent = base::get_first(new_root);
        new_parent->_left     = cur_root;

        cur_root->_parent = new_parent;
        cur_root->_right  = nullptr;

        if (cur_parent == nullptr)
            return;

        (cur_root == cur_parent->_right
            ? cur_parent->_right
            : cur_parent->_left
        ) = new_root;
    }

    void
    rotate_right(node_type* cur_root, node_type* new_root)
    {
        node_type* cur_parent = cur_root->_parent;
        new_root->_parent     = cur_parent;

        node_type* new_parent = base::get_last(new_root);
        new_parent->_right    = cur_root;

        cur_root->_parent = new_parent;
        cur_root->_left   = nullptr;

        if (cur_parent == nullptr)
            return;

        (cur_root == cur_parent->_right
            ? cur_parent->_right
            : cur_parent->_left
        ) = new_root;
    }
};

