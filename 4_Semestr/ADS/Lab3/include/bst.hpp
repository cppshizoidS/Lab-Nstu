#pragma once

#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iterator>
#include <utility>
#include <vector>


template <typename Key = int, typename Compare = std::less<Key>>
class bst
{
public: // definitions
    struct bst_iterator;

private: // definitions
    struct node;

public: // typedefs
    using value_type             = Key;
    using size_type              = size_t;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using pointer                = value_type*;
    using const_pointer          = const value_type*;
    using iterator               = bst_iterator;
    using const_iterator         = const iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using node_type              = node;

public:
    struct bst_iterator
    {
        friend bst;

    public: // typedefs
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = ptrdiff_t;
        using value_type        = Key;
        using reference         = value_type&;
        using pointer           = value_type*;

        node_type* _node;

    public: // ctors
        explicit bst_iterator(node_type* node = nullptr) : _node(node) {}

        bst_iterator(const_iterator& other) : bst_iterator(other._node) {}

    public: // operators
        iterator&
        operator++()
        {
            // если есть правый дочерний узел
            if (_node->_right != nullptr) {
                // то переходим на него
                _node = _node->_right;
                // и спускаемся в самую дальную его левую часть
                while (_node->_left != nullptr)
                    _node = _node->_left;
            }
            // иначе - поднимаемся на уровень выше
            else {
                node_type* parent = _node->_parent;

                // пока текущий узел является правым для родительского
                while (parent != nullptr && _node == parent->_right) {
                    // поднимаем текущий указатель на один уровень вверх
                    _node  = parent;
                    // поднимаем также родительский указатель
                    parent = parent->_parent;
                }

                if (parent != nullptr && _node->_right != parent)
                    _node = parent;
            }

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
            // если доступен левый дочерний узел
            if (_node->_left != nullptr) {
                node_type* prev = _node->_left;

                // спуск к самому правому узлу ветки prev
                while (prev->_right != nullptr)
                    prev = prev->_right;
                // сохраняем позицию
                _node = prev;
            }
            // иначе поднимаемся на уровень выше
            else {
                node_type* parent = _node->_parent;

                // поднимамся на уровень выше до тех пор
                // пока текущий узел для своего родителя левый
                while (parent != nullptr && _node == parent->_left) {
                    _node  = parent;
                    parent = parent->_parent;
                }
                _node = parent;
            }

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
        { return _node->_value; }

        const_reference
        operator*() const
        { return _node->_value; }

        pointer
        operator->()
        { return &_node->_value; }

        const_pointer
        operator->() const
        { return &_node->_value; }

        bool
        operator==(const_iterator& other) const
        { return _node == other._node; }
    };

private:
    struct node
    {
        friend bst;

        /* size_t _weight     = 1; */
        value_type _value{};
        node_type* _parent = nullptr;
        node_type* _left   = nullptr;
        node_type* _right  = nullptr;

    public: // ctors
        node() = default;

        explicit node(
            const_reference value,
            node_type* parent = nullptr,
            node_type* left   = nullptr,
            node_type* right  = nullptr
        )
            : _value(value)
            , _parent(parent)
            , _left(left)
            , _right(right)
        {}

    public: // operators
        auto
        operator<=>(const node& other) const
        { return _value <=> other._value; }
    };

protected: // fields
    size_type _size   = 0;
    node_type* _base  = nullptr;
    node_type* _root  = nullptr;
    node_type* _first = nullptr;
    node_type* _last  = nullptr;

public: // ctors
    bst()
    {
        _base  = new node_type();
        _first = _base;
        _last  = _base;
    }

    bst(const bst& other) : bst()
    { *this = other; }

    bst(bst&& other)
        : _size(std::exchange(other._size,   0))
        , _base(std::exchange(other._base,   nullptr))
        , _root(std::exchange(other._root,   nullptr))
        , _first(std::exchange(other._first, nullptr))
        , _last(std::exchange(other._last,   nullptr))
    {}

    ~bst()
    {
        clear();
        delete _base;
    }

public: // operators
    bst&
    operator=(const bst& other)
    {
        clear();

        for (auto& item : other)
            insert(item);

        return *this;
    }

    bst&
    operator=(bst&& other)
    {
        clear();

        _size  = std::exchange(other._size,  0);
        _base  = std::exchange(other._base,  nullptr);
        _root  = std::exchange(other._root,  nullptr);
        _first = std::exchange(other._first, nullptr);
        _last  = std::exchange(other._last,  nullptr);

        return *this;
    }

public: // methods
    virtual iterator
    insert(const_reference value)
    {
        if (_root != nullptr)
            return insert(iterator(_root), value);

        _root  = new node_type(value, _base);
        _first = _root;
        _last  = _root;
        _size++;

        _base->_right = _first;
        _base->_left  = _last;
        return iterator(_root);
    }

    virtual void
    erase(const_reference value)
    { erase(find(value)); }

    virtual void
    erase(iterator pos)
    {
        if (pos == end())
            return;

        node_type* node   = pos._node;
        node_type* parent = node->_parent;

        if (node == _root)
            _root = _root->_right;
        else if (node == _first)
            _first = (++begin())._node;
        else if (node == _last)
            _last = (++rbegin()).base()._node;

        // no cildren
        if ((node->_left == nullptr) && (node->_right == nullptr)) {
            (node == parent->_left
                ? parent->_left
                : parent->_right
            ) = nullptr;
        }
        // left to right
        else if ((node->_left != nullptr) && (node->_right != nullptr)) {
            node_type* new_parent      = get_first(node->_right);
            new_parent->_left          = std::exchange(node->_left, nullptr);
            new_parent->_left->_parent = new_parent;

            node->_right->_parent = parent;

            (node == parent->_left
                ? parent->_left
                : parent->_right
            ) = node->_right;
        }
        // one child
        else if (parent != nullptr) {
            node_type* child = (
                node->_left == nullptr
                ? node->_right
                : node->_left
            );
            child->_parent   = parent;

            (node == parent->_left
                ? parent->_left
                : parent->_right
            ) = child;
        }

        _size--;
        delete node;
    }

#if defined TLR || defined LRT || defined LTR
    std::vector<value_type>
    output() const
    { return __output(_root, true); }
#endif

#if defined INDEX_OF || defined COUNT_MORE_THAN
    size_t
    index_of(const_reference key)
    {
        size_t index      = 0;
        iterator pos      = find(key);
        node_type* node   = pos._node;
        node_type* parent = node->_parent;

        if (node->_left != nullptr)
            index = tree_size(node->_left);

        while (parent != _base) {
            if (node == parent->_right) {
                index++;
                if (parent->_left != nullptr)
                    index += tree_size(parent->_left);
            }

            node   = parent;
            parent = parent->_parent;
        }

        return index;
    }
#endif

#ifdef COUNT_MORE_THAN
    size_type
    count_more_than(const_reference value)
    { return _size - (index_of(value) + 1); }

    size_type
    count_more_than(iterator pos)
    { return _size - (index_of(*pos) + 1); }
#endif

#ifdef BALANCE_FACTOR
    ssize_t
    balance_factor() const
    { return balance_factor(iterator(_root)); }

    ssize_t
    balance_factor(iterator pos) const
    {
        if (size() == 0)
            return 0;
        return tree_height(pos._node->_right) - tree_height(pos._node->_left);
    }
#endif

    void
    clear()
    {
        delete_tree(_root);
        _first = _base;
        _last  = _base;
        _size  = 0;
        _root  = nullptr;
    }

    size_type
    size() const
    { return _size; }

#ifdef RECURSIVE
    iterator
    find(const_reference key)
    {
        if (_root == nullptr)
            return end();
        return bsearch(iterator(_root), key);
    }

    const_iterator
    find(const_reference key) const
    {
        if (_root == nullptr)
            return end();
        return bsearch(const_iterator(_root), key);
    }
#elif defined ITERATIVE
    iterator
    find(const_reference key)
    {
        for (auto iter = begin(); iter != end(); ++iter) {
            if (*iter == key)
                return iter;
        }

        return end();
    }

    const_iterator
    find(const_reference key) const
    { return find(key); }
#endif

    bool
    contains(const_reference key)
    { return find(key) != end(); }

    bool
    empty() const
    { return _size == 0; }

#ifdef OPTIONAL
    reference
    front()
    { return _first->_value; }

    reference
    back()
    { return _last->_value; }
#endif

    iterator
    begin()
    { return iterator(_first); }

    iterator
    end()
    { return iterator(_base); }

    const_iterator
    begin() const
    { return iterator(_first); }

    const_iterator
    end() const
    { return iterator(_base); }

    reverse_iterator
    rbegin()
    { return reverse_iterator(--end()); }

    reverse_iterator
    rend()
    { return reverse_iterator(--begin()); }

    const_reverse_iterator
    rbegin() const
    { return const_reverse_iterator(end()); }

    const_reverse_iterator
    rend() const
    { return const_reverse_iterator(begin()); }

#ifdef MERGE
    void
    merge(const bst& other)
    {
        for (const auto& item : other)
            insert(other);
    }
#endif

#ifdef GREATER_TO_ROOT
    template <typename Val> requires std::is_convertible_v<Val, value_type>
    iterator
    greater_to_root(Val&& value)
    {
        auto iter = find(std::forward<value_type>(value));

        if (iter == end() || ++iter == end())
            return end();

        if (iter._node->_value > _root->_value) {
            node_type* leftest = get_first(iter._node);
            leftest->_parent   = _base;
            leftest->_left     = _root;
            _root->_parent     = leftest;
            _root->_right      = nullptr;
        } else {
            node_type* rightest = get_last(iter._node);
            rightest->_parent   = _base;
            rightest->_right    = _root;
            _root->_parent      = rightest;
            _root->_left        = nullptr;
        }

        _root = iter._node;
        return iterator(_root);
    }
#endif

protected:
    static node_type*
    get_first(node_type* node)
    {
        while (node->_left != nullptr)
            node = node->_left;
        return node;
    }

    static node_type*
    get_last(node_type* node)
    {
        while (node->_right != nullptr)
            node = node->_right;
        return node;
    }

    template <typename Val> requires std::is_convertible_v<Val, value_type>
    iterator
    raw_insert(node_type*& child, node_type* parent, Val&& value)
    {
        child = new node_type(value, parent);

        if (Compare{}(child->_value, _first->_value)) {
            _first        = child;
            _base->_right = _first;
        } else if (Compare{}(_last->_value, child->_value)) {
            _last        = child;
            _base->_left = _last;
        }

        _size++;
        return iterator(child);
    }

private:
    iterator
    insert(iterator hint, const_reference value)
    {
        if (value == *hint)
            return hint;

        node_type* node   = hint._node;
        node_type*& child = (
            Compare{}(value, *hint)
            ? node->_left
            : node->_right
        );

        return (
            child == nullptr
            ? raw_insert(child, node, value)
            : insert(iterator(child), value)
        );
    }

#ifdef RECURSIVE
    template <typename Iterator> requires
        std::is_convertible_v<Iterator, iterator>
    Iterator
    bsearch(Iterator hint, const_reference key)
    {
        if ((hint == end()) || (*hint == key))
            return hint;

        node_type* node = (
            Compare{}(key, *hint)
            ? hint._node->_left
            : hint._node->_right
        );

        if (node == nullptr)
            return end();

        return bsearch(Iterator(node), key);
    }
#endif

    size_type
    delete_tree(node_type* node)
    {
        if (node == nullptr)
            return 0;

        size_t deleted = 1
            + delete_tree(node->_left)
            + delete_tree(node->_right);

        delete node;
        return deleted;
    }

    static size_type
    tree_size(node_type* node)
    {
        return (
            node == nullptr
            ? 0
            : 1 + tree_size(node->_left) + tree_size(node->_right)
        );
    }

#ifdef BALANCE_FACTOR
    static size_type
    tree_height(node_type* node)
    {
        if (node == nullptr)
            return 0;

        size_t left_h  = 1 + tree_height(node->_left);
        size_t right_h = 1 + tree_height(node->_right);

        return (left_h > right_h ? left_h : right_h);
    }
#endif

    std::vector<value_type>
    __output(node_type* node, bool first = false) const
    {
        static std::vector<value_type> items;
        if (first)
            items.clear();
        items.reserve(size());

        if (node != nullptr) {
#ifdef TLR
            items.push_back(node->_value);
#endif

            if (node->_left != nullptr)
                __output(node->_left);

#ifdef LTR
            items.push_back(node->_value);
#endif

            if (node->_right != nullptr)
                __output(node->_right);

#ifdef LRT
            items.push_back(node->_value);
#endif
        }

        return items;
    }
};

