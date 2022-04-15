#pragma once

#include <cstddef>
#include <functional>
#include <iterator>
#include <utility>


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

    private: // fields
        node_type* _node;

    public: // ctors
        explicit bst_iterator(node_type* node) : _node(node) {}

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
                while (_node == parent->_right) {
                    // поднимаем текущий указатель на один уровень вверх
                    _node  = parent;
                    // поднимаем также родительский указатель
                    parent = parent->_parent;
                }

                if (_node->_right != parent)
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
                while (_node == parent->_left) {
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
        operator*() const;

        pointer
        operator->();

        const_pointer
        operator->() const;

        bool
        operator==(const_iterator& other)
        { return _node == other._node; }

        bool
        operator!=(const_iterator& other)
        { return !(*this == other); }
    };

private:
    struct node
    {
        friend bst;

    private: // fields
        value_type _value;
        node_type* _parent = nullptr;
        node_type* _left   = nullptr;
        node_type* _right  = nullptr;

    public: // ctors
        node() = default;

        explicit node(
            value_type&& value,
            node_type* parent = nullptr,
            node_type* left   = nullptr,
            node_type* right  = nullptr
        )
            : _value(std::forward<Key>(value))
            , _parent(parent)
            , _left(left)
            , _right(right)
        {}

    public: // operators
        auto
        operator<=>(const node& other) const
        { return _value <=> other._value; }
    };

private: // fields
    size_type _size   = 0;
    node_type* _base  = nullptr;
    node_type* _root  = nullptr;
    node_type* _first = nullptr;
    node_type* _last  = nullptr;

public: // ctors
    bst()
    { _base = new node_type(); }

    bst(const bst& other);

    bst(bst&& other)
        : _root(std::exchange(other._root, nullptr))
        , _size(std::exchange(other._root, 0))
    {}

    ~bst()
    {
        clear();
        delete _base;
    }

public: // operators
    bst&
    operator=(const bst& other);

    bst&
    operator=(bst&& other);

public: // methods
    iterator
    insert(value_type&& value)
    {
        if (_root != nullptr)
            return insert(iterator(_root), std::forward<value_type>(value));

        _root  = new node_type(std::forward<value_type>(value), _base);
        _first = _root;
        _last  = _root;
        _size++;

        _base->_right = _first;
        _base->_left  = _last;
        return iterator(_root);
    }

    iterator
    insert(iterator hint, value_type&& value)
    {
        if (value == *hint)
            return hint;

        node_type*& node = hint._node;
        node_type*& child = (
            value < *hint
            ? node->_left
            : node->_right
        );

        if (child == nullptr) {
            child = new node_type(std::forward<value_type>(value), node);
            if (*child < *_first) {
                _first        = child;
                _base->_right = _first;
            } else if (*child > *_last) {
                _last        = child;
                _base->_left = _last;
            }

            _size++;
            return iterator(child);
        }

        return insert(iterator(child), std::forward<value_type>(value));
    }

    void
    erase(iterator& pos)
    { erase(*pos); }

    size_type
    erase(value_type&& key);

    void
    clear()
    {
        /* auto prev = begin(); */
        /* auto iter = begin(); */
        /* ++iter; */

        /* for (; iter != end(); ++iter) { */
        /*     delete prev._node; */
        /*     prev = iter; */
        /*     /1* prev._node = nullptr; *1/ */
        /* } */

        /* delete iter._node; */
    }

    size_type
    size() const
    { return _size; }

    iterator
    find(const_reference key);

    const_iterator
    find(const_reference key) const;

    bool
    contains() const;

    bool
    empty() const
    { return _size == 0; }

    value_type&
    front()
    { return _first->_value; }

    value_type&
    back()
    { return _last->_value; }

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
    { return reverse_iterator(end()); }

    reverse_iterator
    rend()
    { return reverse_iterator(begin()); }

    const_reverse_iterator
    rbegin() const
    { return const_reverse_iterator(end()); }

    const_reverse_iterator
    rend() const
    { return const_reverse_iterator(begin()); }
};

