template<typename T>
class evector
{
public:
    struct iterator;

    enum class error
    {
        out_of_range,
        invalid_param
    };

    using value_type             = T;
    using size_type              = size_t;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using pointer                = value_type*;
    using const_pointer          = const value_type*;
    using const_iterator         = const iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    struct iterator
    {
        friend evector;

    public:
        using value_type        = T;
        using iterator_category = std::random_access_iterator_tag;
        using difference_type   = ptrdiff_t;

        iterator(pointer ptr) : _ptr(ptr) {}

        iterator(const iterator& other) : _ptr(other._ptr) {}

        iterator&
        operator++();

        iterator
        operator++(int);

        iterator&
        operator--();

        iterator
        operator--(int);

        reference
        operator*()
        { return *_ptr; }

        const_reference
        operator*() const
        { return *_ptr; }

        pointer
        operator->()
        { return _ptr; }

        const_pointer
        operator->() const
        { return _ptr; }

        auto
        operator<=>(const_iterator& other) const = default;

        iterator
        operator+(size_type offset) const
        { return iterator(_ptr + offset); }

        iterator
        operator-(size_type offset) const
        { return iterator(_ptr - offset); }

    private:
        pointer _ptr = nullptr;
    };

    evector()
    { reserve(1); }

    evector(size_type cap) : _size(cap)
    { reserve(cap); }

    evector(const evector& src)
    { *this = src; }

    evector(evector&& src) noexcept
    { *this = std::move(src); }

    evector(const std::initializer_list<value_type>& init_list)
        : evector(init_list.size())
    {
        size_type n = 0;
        for (const auto& item : init_list)
            _data[n++] = item;
    }

    ~evector()
    {
        clear();
        delete[] _data;
    }

    reference
    operator[](size_type pos)
    { return _first[pos]; }

    const_reference
    operator[](size_type pos) const
    { return _first[pos]; }

    evector&
    operator=(const evector& other);

    evector&
    operator=(evector&& other) noexcept;

    friend std::ostream&
    operator<<(std::ostream& os, const evector& rhs);

    iterator
    begin()
    { return iterator(_first); }

    iterator
    end()
    { return iterator(_last + 1); }

    const_iterator
    begin() const
    { return iterator(_first); }

    const_iterator
    end() const
    { return iterator(_last + 1); }

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

    reference
    front()
    { return *_first; }

    reference
    back()
    { return *_last; }

    const_reference
    front() const
    { return *_first; }

    const_reference
    back() const
    { return *_last; }

    size_type
    size() const
    { return _size; }

    size_type
    capacity() const
    { return _capacity; }

    bool
    empty() const
    { return !_size; }

    void
    clear()
    {
        _first = nullptr;
        _last  = nullptr;
        _size  = 0;
    }

    void
    reserve(size_type new_cap);

    tl::expected<pointer, error>
    at(size_type pos);

    tl::expected<const_pointer, error>
    at(size_type pos) const;

    tl::expected<size_type, error>
    shift(ssize_t offset)
    { return shift(*get_index(_first), _size, offset); }

    tl::expected<size_type, error>
    shift(size_type start, size_type end, ssize_t offset);

    void
    shrink_to_fit();

    template<typename U>
    void
    push_front(U&& value);

    template<typename U>
    void
    push_back(U&& value);

    void
    pop_front()
    {
        _first++;
        _size--;
    }

    void
    pop_back()
    {
        _last--;
        _size--;
    }

    iterator
    find(const_reference value) const
    { return find(begin(), end(), value); }

    iterator
    find(iterator from, const_iterator to, const_reference value) const;

    template<typename U>
    iterator
    insert(iterator pos, U&& value);

    iterator
    erase(iterator pos);

private:
    size_type _size     = 0;
    size_type _capacity = 0;
    pointer _data       = nullptr;
    pointer _first      = nullptr;
    pointer _last       = nullptr;

    void
    allocate(size_type cap)
    {
        _data     = new value_type[cap + 1]();
        _capacity = cap;
    }

    void
    shift_front(size_type start, size_type end, size_type offset);

    void
    shift_back(size_type start, size_type end, size_type offset);

    tl::expected<size_type, error>
    get_index(const_pointer ptr);
};

