#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <optional>
#include <utility>


template <typename Key = int, typename Compare = std::less<Key>>
class t234
{
private:
    struct Node;

public:
    struct Iterator;

public: // typedefs
    using value_type             = Key;
    using node_type              = Node;
    using size_type              = size_t;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using pointer                = value_type*;
    using const_pointer          = const value_type*;
    using iterator               = Iterator;
    using const_iterator         = const iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
    struct Iterator
    {
        friend t234;

    public: // typedefs
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = ptrdiff_t;
        using value_type        = Key;

    private: // fields
        node_type* node_ = nullptr;
        size_t key_num_  = 0;

    public: // ctors
        Iterator(node_type* node, size_t key_num)
            : node_(node), key_num_(key_num) {}

        Iterator(const_iterator& other) = default;

    public: // operators
        iterator&
        operator++()
        {
            if (node_->is_leaf()) {
                if (key_num_ + 1 < node_->size()) {
                    ++key_num_;
                    return *this;
                }
            }

            *this = get_next(node_, key_num_);
            return *this;
        }

        iterator
        operator++(int)
        {
            auto old = *this;
            ++(*this);
            return old;
        }

        iterator&
        operator--()
        {
            if (node_->is_leaf()) {
                if (key_num_ > 0) {
                    --key_num_;
                    return *this;
                }
            }

            *this = get_prev(node_, key_num_);
            return *this;
        }
        
        iterator
        operator--(int)
        {
            auto old = *this;
            --(*this);
            return old;
        }

        reference
        operator*()
        { return *node_->keys_[key_num_]; }

        const_reference
        operator*() const
        { return *node_->keys_[key_num_]; }

        pointer
        operator->()
        { return &*node_->keys_[key_num_]; }

        const_pointer
        operator->() const
        { return &*node_->keys_[key_num_]; }

        bool
        operator==(const_iterator& other) const
        { return node_ == other.node_; }

    private:
        static iterator
        get_next(node_type* node, size_t key_num)
        {
            auto child = node->nodes_[key_num + 1];
            if (child != nullptr)
                return iterator(child->get_leftest(), 0);

            size_t index;
            do {
                if (node->parent_ == nullptr)
                    return iterator(nullptr, 0);

                index = node->parent_->index_of(node);
                node  = node->parent_;
            } while (index == node->size());

            return iterator(node, index);
        }

        static iterator
        get_prev(node_type* node, size_t key_num)
        {
            auto child = node->nodes_[key_num];
            if (child != nullptr) {
                node_type* prev = child->get_rightest();
                return iterator(prev, prev->size() - 1);
            }

            size_t index;
            do {
                if (node->parent_ == nullptr)
                    return iterator(nullptr, 0);

                index = node->parent_->index_of(node);
                node  = node->parent_;
            } while (index == 0);

            return iterator(node, index - 1);
        }
    };

private:
    struct Node
    {
        friend t234;

    private:
        using key_opt = std::optional<value_type>;
        using Keys    = std::array<key_opt, 3>;
        using Nodes   = std::array<node_type*, 4>;

        Keys keys_{};
        Nodes nodes_{};
        node_type* parent_ = nullptr;

    public:
        Node(Keys&& keys) : keys_(std::move(keys)) {}

        Node(value_type&& key)
        { keys_[0] = std::move(key); }

        Node(key_opt&& key)
        { keys_[0] = std::move(key); }

        bool
        is_leaf() const
        {
            size_t n = 0;
            for (; n < nodes_.size() && !nodes_[n]; ++n);
            return n == nodes_.size();
        }

        /**
         * @return number of sotred keys
         */
        size_t
        size() const
        {
            size_t n = 0;
            for (; n < keys_.size() && keys_[n]; ++n);
            return n;
        }

        /**
         * @brief check it's a 4-node
         */
        bool
        full() const
        {
            size_t n = 0;
            for (; n < keys_.size() && keys_[n]; ++n);
            return n == keys_.size();
        }

        /**
         * @brief checks on having a key
         *
         * @return index of the key or std::nullopt_t
         */
        std::optional<size_t>
        has(const_reference key) const
        {
            size_t n = 0;
            for (; (n < keys_.size()) && (keys_[n] != key); ++n);
            
            if (n != keys_.size())
                return n;
            return {};
        }

        /**
         * @details calling this method you guarantee the node is not
         * full and the node is not a leaf
         *
         * @return inserted key index
         */
        size_t
        raw_insert(value_type&& key)
        {
            size_t pos = 0;
            for (; (pos < keys_.size()) && keys_[pos]; ++pos);

            keys_[pos] = std::move(key);
            return sort_shift(pos);
        }

        /**
         * @brief splits one 4-node into two 2-nodes
         *
         * @details Every of 3 keys is being placed into its own node.
         * If original node doesn't have a parent, it will be created.
         * k0 stays in original node, optionally getting a new parent.
         * k1 becomes a new separating key for its neighbors.
         * k2 is being placed into a new node that will become
         * an original node's right sibling.
         *
         * @return pointer to node having become a new parent for splitted nodes
         */
        node_type*
        split()
        {
            size_t index = 0;

            if (parent_ == nullptr)
                parent_ = new node_type(std::exchange(keys_[1], {}));
            else
                index = parent_->raw_insert(*std::exchange(keys_[1], {}));

            auto right     = new node_type(std::exchange(keys_[2], {}));
            right->parent_ = parent_;

            right->nodes_[0] = std::exchange(nodes_[2], nullptr);
            if (right->nodes_[0] != nullptr)
                right->nodes_[0]->parent_ = right;

            right->nodes_[1] = std::exchange(nodes_[3], nullptr);
            if (right->nodes_[1] != nullptr)
                right->nodes_[1]->parent_ = right;

            parent_->nodes_[index]     = this; // left
            parent_->nodes_[index + 1] = right;

            return parent_;
        }

        /**
         * @brief shifts a key (with subnodes) to its order place
         *
         * @return index of the key's new position
         */
        size_t
        sort_shift(size_t pos)
        {
            size_t new_pos = pos;

            for (; pos > 0; --pos) {
                if (keys_[pos] < keys_[pos - 1]) {
                    keys_[pos - 1].swap(keys_[pos]);
                    std::swap(nodes_[pos], nodes_[pos + 1]);
                    --new_pos;
                }
            }

            return new_pos;
        }

        /**
         * @brief find node to insert the key
         *
         * @details there is no sense to consider the case when node is empty,
         * because the only case where it possible is node is root
         * (also calling this method you guarantee the key is not dublicate)
         *
         * @return pointer to insertion node
         */
        node_type*
        find_yourself(reference key)
        {
            if (key < keys_[0])
                return nodes_[0];

            if (keys_[1]) {
                if (key < keys_[1])
                    return nodes_[1];

                if (keys_[2]) {
                    if (key < keys_[2])
                        return nodes_[2];
                    return nodes_[3];
                }

                return nodes_[2];
            }

            return nodes_[1];
        }

        node_type*
        get_leftest()
        {
            return (
                nodes_.front() == nullptr
                ? this
                : nodes_.front()->get_leftest()
            );
        }

        node_type*
        get_rightest()
        {
            return (
                nodes_[size()] == nullptr
                ? this
                : nodes_[size()]->get_rightest()
            );
        }

        /**
         * @brief merging the node with its child nodes
         *
         * @details calling this method you guarantee the current node
         * and its child nodes are 2-nodes
         */
        void
        shrink_tree()
        {
            node_type* left  = nodes_[0];
            node_type* right = nodes_[1];

            keys_[1] = std::exchange(keys_[0], {});
            keys_[0] = std::exchange(left->keys_[0], {});
            keys_[2] = std::exchange(right->keys_[0], {});

            nodes_[0] = left->nodes_[0];
            nodes_[1] = left->nodes_[1];
            nodes_[2] = right->nodes_[0];
            nodes_[3] = right->nodes_[1];

            if (nodes_[0] != nullptr)
                nodes_[0]->parent_ = this;
            if (nodes_[1] != nullptr)
                nodes_[1]->parent_ = this;

            if (nodes_[2] != nullptr)
                nodes_[2]->parent_ = this;
            if (nodes_[3] != nullptr)
                nodes_[3]->parent_ = this;

            delete left;
            delete right;
        }

        /**
         * @details Calling this method you guarantee the current node
         * is a 3-node at least.
         * Also both of param nodes must be siblings
         */
        void
        merge_left(node_type* left, node_type* right)
        {
            left->keys_[1] = std::exchange(keys_[0], {});
            left->keys_[2] = std::exchange(right->keys_[0], {});

            soft_shift();
            nodes_[0] = left;

            left->nodes_[2] = right->nodes_[0];
            if (left->nodes_[2] != nullptr)
                left->nodes_[2]->parent_ = left;

            left->nodes_[3] = right->nodes_[1];
            if (left->nodes_[3] != nullptr)
                left->nodes_[3]->parent_ = left;

            delete right;
        }

        void
        merge_right(node_type* left, node_type* right)
        {
            size_t index = index_of(left);

            right->keys_[2] = std::exchange(right->keys_[0], {});
            right->keys_[1] = std::exchange(keys_[index], {});
            right->keys_[0] = std::exchange(left->keys_[0], {});

            soft_shift();
            nodes_[index] = right;

            right->nodes_[3] = right->nodes_[1];
            right->nodes_[2] = right->nodes_[0];
            right->nodes_[1] = left->nodes_[1];
            right->nodes_[0] = left->nodes_[0];

            if (right->nodes_[0] != nullptr)
                right->nodes_[0]->parent_ = right;

            if (right->nodes_[1] != nullptr)
                right->nodes_[1]->parent_ = right;

            delete left;
        }

        void
        soft_shift()
        {
            for (size_t n = 0; n + 1 < keys_.size(); ++n) {
                if (!keys_[n]) {
                    keys_[n].swap(keys_[n + 1]);
                    nodes_[n]     = nodes_[n + 1];
                    nodes_[n + 1] = nodes_[n + 2];
                }
            }
        }

        /**
         * @param left - 3-4-node
         * @param right - 2-node
         */
        void
        rotate_right(node_type* left, node_type* right)
        {
            right->keys_[1]  = std::exchange(right->keys_[0], {});
            right->nodes_[2] = right->nodes_[1];
            right->nodes_[1] = right->nodes_[0];

            right->keys_[0]  = std::exchange(keys_[0], {});
            right->nodes_[0] = left->nodes_[left->size()];

            if (right->nodes_[0] != nullptr)
                right->nodes_[0]->parent_ = right;

            keys_[0] = std::exchange(left->keys_[left->size() - 1], {});
        }

        /**
         * @param left - 2-node
         * @param right - 3-4-node
         */
        void
        rotate_left(node_type* left, node_type* right)
        {
            size_t index = index_of(left);

            left->keys_[1]  = std::exchange(keys_[index], {});
            left->nodes_[2] = right->nodes_[0];

            keys_[index] = std::exchange(right->keys_[0], {});
            right->soft_shift();
        }

        size_t
        index_of(const node_type* subnode)
        {
            size_t n = 0;

            for (; n < nodes_.size(); ++n) {
                if (nodes_[n] == subnode)
                    break;
            }

            return n;
        }

        node_type*
        left_sibling()
        {
            if (parent_ == nullptr)
                return nullptr;

            size_t index = parent_->index_of(this);

            if (0 < index && index <= parent_->size())
                return parent_->nodes_[index - 1];
            return nullptr;
        }

        node_type*
        right_sibling()
        {
            if (parent_ == nullptr)
                return nullptr;

            size_t index = parent_->index_of(this);

            if (0 <= index && index < parent_->size())
                return parent_->nodes_[index + 1];
            return nullptr;
        }
    };

private:
    size_type size_  = 0;
    node_type* root_ = nullptr;

public:
    t234() = default;

    t234(const t234& other)
    {
        clear();
        for (const_reference item : other)
            insert(item);
    }

    ~t234()
    { clear(); }

    template <typename T> requires std::is_convertible_v<T, value_type>
    iterator
    insert(T&& key)
    {
        if (root_ == nullptr) {
            ++size_;
            root_ = new node_type(std::forward<value_type>(key));
            return iterator(root_, 0);
        }

        return insert(root_, std::forward<value_type>(key));
    }

    template <typename T> requires std::is_convertible_v<T, value_type>
    iterator
    insert(node_type* node, T&& key)
    {
        if (auto index = node->has(key))
            return iterator(node, *index);

        if (node->full()) {
            node_type* parent = node->split();

            if (node == root_)
                root_ = parent;

            return insert(parent, std::forward<value_type>(key));
        }

        if (node->is_leaf()) {
            ++size_;
            size_t index = node->raw_insert(std::forward<value_type>(key));
            return iterator(node, index);
        }

        node_type* insert_node = node->find_yourself(key);
        return insert(insert_node, std::forward<value_type>(key));
    }

    iterator
    begin()
    {
        if (empty())
            return end();
        return iterator(root_->get_leftest(), 0);
    }

    iterator
    end()
    { return iterator(nullptr, 0); }

    iterator
    rbegin()
    {
        if (empty())
            return end();
        node_type* rightest = root_->get_rightest();
        return (iterator(rightest, rightest->size() - 1));
    }

    iterator
    rend()
    { return end(); }

#ifdef ITERATIVE
    template <typename T> requires std::is_convertible_v<T, value_type>
    iterator
    find(T&& key)
    {
        for (auto iter = begin(); iter != end(); ++iter) {
            if (key == *iter)
                return iter;
        }
        return end();
    }
#endif

#ifdef RECURSIVE
    template <typename T> requires std::is_convertible_v<T, value_type>
    iterator
    find(T&& key)
    { return find(std::forward<value_type>(key), root_); }

    template <typename T> requires std::is_convertible_v<T, value_type>
    iterator
    find(T&& key, node_type* node)
    {
        if (auto index = node->has(key))
            return iterator(node, *index);

        node_type* insert_node = node->find_yourself(key);
        if (insert_node != nullptr)
            return find(std::forward<value_type>(key), insert_node);

        return end();
    }
#endif

    void
    erase(const_iterator pos)
    {
        node_type* node   = pos.node_;
        node_type* parent = node->parent_;

        if (node->is_leaf()) {
            if (node->size() > 1 || node == root_) {
                --size_;
                node->keys_[pos.key_num_].reset();
                return node->soft_shift();
            }

            node_type* l_sibling = node->left_sibling();
            node_type* r_sibling = node->right_sibling();

            // shrink or rotate
            if (parent->size() == 1) {
                node_type* sibling = (
                    r_sibling == nullptr ? l_sibling : r_sibling
                );

                // shrink case
                if (sibling->size() == 1) {
                    parent->shrink_tree();
                    return erase(iterator(parent, (
                        sibling == r_sibling ? 0 : 2
                    )));
                }

                // rotate case
                if (r_sibling != nullptr && r_sibling->size() > 1) {
                    parent->rotate_left(node, r_sibling);
                    return erase(pos);
                }

                if (l_sibling != nullptr && l_sibling->size() > 1) {
                    parent->rotate_right(l_sibling, node);
                    return erase(iterator(node, pos.key_num_ + 1));
                }
            }

            size_t index = parent->index_of(node);

            // merge case
            if (l_sibling != nullptr && l_sibling->size() == 1) {
                parent->merge_right(l_sibling, node);
                return erase(iterator(node, node->size() - 1));
            }

            if (r_sibling != nullptr && r_sibling->size() == 1) {
                parent->merge_left(node, r_sibling);
                return erase(iterator(node, 0));
            }

            // rotate case
            if (r_sibling != nullptr && r_sibling->size() > 1) {
                parent->rotate_left(node, r_sibling);
                return erase(pos);
            }

            if (l_sibling != nullptr && l_sibling->size() > 1) {
                parent->rotate_right(l_sibling, node);
                return erase(iterator(node, pos.key_num_ + 1));
            }
        }

        if (node->size() > 1) {
            node_type* left  = node->nodes_[pos.key_num_];
            node_type* right = node->nodes_[pos.key_num_ + 1];

            // swap case
            if (left->size() > 1) {
                auto& pred = left->keys_[left->size() - 1];
                pred.swap(node->keys_[pos.key_num_]);
                return erase(iterator(left, left->size() - 1));
            }

            // rotate case
            if (right->size() > 1) {
                node->rotate_left(left, node->nodes_[pos.key_num_ + 1]);
                return erase(iterator(left, left->size() - 1));
            }

            // merge case
            if (left->size() == 1 && right->size() == 1) {
                node->merge_right(left, right);
                return erase(iterator(right, 1));
            }
        }

        if (node->size() == 1) {
            if (parent == nullptr) {
                node->shrink_tree();
                return erase(iterator(node, 1));
            }

            if (parent->size() == 1 && parent->nodes_[1]->size() == 1) {
                parent->shrink_tree();
                return erase(iterator(parent, 0));
            }
        }
    }

    void
    clear()
    { size_ -= delete_node(root_); }

    inline size_t
    size() const
    { return size_; }

    inline bool
    empty() const
    { return size() == 0; }

private:
    static size_t
    delete_node(node_type*& node)
    {
        if (node == nullptr)
            return 0;

        size_t deleted = node->size();
        for (auto& sub : node->nodes_)
            deleted += delete_node(sub);

        delete node;
        node = nullptr;

        return deleted;
    }
};

