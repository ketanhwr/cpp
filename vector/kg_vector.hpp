#pragma once

#include <memory>
#include <iterator>
#include <utility>
#include <initializer_list>
#include <stdexcept>

namespace kg
{

template <typename T, typename Allocator = std::allocator<T>>
class vector
{
    using size_type = std::size_t;
    using value_type = T;
    using Alloc = std::allocator_traits<Allocator>;

  public:
    struct iterator {
        // Needed for LegacyIterator

        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using difference_type = std::ptrdiff_t;

        iterator() : m_ptr{nullptr} {}

        iterator(pointer ptr) : m_ptr{ptr} {}

        iterator(const iterator& other) : m_ptr{other.m_ptr} {}

        iterator& operator=(const iterator& other) {
            m_ptr = other.m_ptr;
            return *this;
        }

        ~iterator() = default;

        reference operator*() { return *m_ptr; }

        iterator& operator++() { m_ptr++; return *this; }

        // Needed for LegacyInputIterator

        friend bool operator==(const iterator& lhs, const iterator& rhs) {
            return (lhs.m_ptr == rhs.m_ptr);
        }

        friend bool operator!=(const iterator& lhs, const iterator& rhs) {
            return !(lhs == rhs);
        }

        pointer operator->() { return m_ptr; }

        // Needed for LegacyBidirectionalIterator

        iterator& operator--() { m_ptr--; return *this; }

        // Needed for RandomAccessIterator

        iterator& operator+=(const difference_type& pos) {
            m_ptr += pos;
            return *this;
        }

        iterator& operator-=(const difference_type& pos) {
            m_ptr -= pos;
            return *this;
        }

        reference operator[](const difference_type& pos) {
            return m_ptr[pos];
        }

        friend bool operator<(const iterator& lhs, const iterator& rhs) {
            return (lhs.m_ptr < rhs.m_ptr);
        }

        iterator operator+(const difference_type& pos) {
            iterator ret{m_ptr};
            return (ret += pos);
        }

        iterator operator-(const difference_type& pos) {
            iterator ret{m_ptr};
            return (ret -= pos);
        }

        difference_type operator-(const iterator& rhs) {
            return (m_ptr - rhs.m_ptr);
        }

      private:
        pointer m_ptr;
    };

    // Default constructor
    vector() : m_start{nullptr}, m_end{nullptr}, m_capacity{0} {}

    // Copy constructor
    vector(const vector& other) : m_capacity{other.m_capacity} {
        m_start = Alloc::allocate(alloc, m_capacity);
        m_end = m_start;
        size_t sz = other.size();
        for (size_t idx = 0; idx < sz; ++idx) {
            this->push_back(other.at(idx));
        }
    }

    // Copy assignment constructor
    vector& operator=(const vector& other) {
        if (this != &other) {
            dealloc();

            m_capacity = other.m_capacity;
            m_start = Alloc::allocate(alloc, m_capacity);
            m_end = m_start;

            size_t sz = other.size();
            for (size_t idx = 0; idx < sz; ++idx) {
                this->push_back(other.at(idx));
            }
        }
        return *this;
    }

    // Move constructor
    vector(vector&& other) {
        m_capacity = std::exchange(other.m_capacity, 0);
        m_start = std::exchange(other.m_start, nullptr);
        m_end = std::exchange(other.m_end, nullptr);
    }

    // Move assignment constructor
    vector& operator=(vector&& other) {
        if (this != &other) {
            dealloc();

            m_capacity = std::exchange(other.m_capacity, 0);
            m_start = std::exchange(other.m_start, nullptr);
            m_end = std::exchange(other.m_end, nullptr);
        }
        return *this;
    }

    // Destructor
    ~vector()
    {
        dealloc();
    }

    // Custom constructors
    explicit vector(size_t n, const value_type& value = value_type()) : m_capacity{n} {
        m_start = Alloc::allocate(alloc, m_capacity);
        m_end = m_start;
        for (size_t idx = 0; idx < n; ++idx) {
            this->push_back(value);
        }
    }

    template <typename InputIt, typename ItCategory = std::iterator_traits<InputIt>::iterator_category>
    vector(InputIt first, InputIt last)
        : m_start{nullptr}, m_end{nullptr}, m_capacity{0}
    {
        for (auto it = first; it != last; ++it) {
            this->push_back(*it);
        }
    }

    vector(std::initializer_list<value_type> init) : m_start{nullptr}, m_end{nullptr} {
        m_capacity = init.size();

        m_start = Alloc::allocate(alloc, m_capacity);
        m_end = m_start;

        for (auto it = init.begin(); it != init.end(); ++it) {
            this->push_back(*it);
        }
    }

    // Current size
    size_type size() const
    {
        return (m_end - m_start);
    }

    // Current capacity
    size_type capacity() const
    {
        return m_capacity;
    }

    value_type& operator[](const size_t& idx)
    {
        return m_start[idx];
    }

    const value_type& operator[](const size_t& idx) const
    {
        return m_start[idx];
    }

    value_type& at(const size_t& idx)
    {
        if (idx >= size()) {
            throw std::out_of_range("Accessing out-of-range element");
        }

        return m_start[idx];
    }

    const value_type& at(const size_t& idx) const
    {
        if (idx >= size()) {
            throw std::out_of_range("Accessing out-of-range element");
        }

        return m_start[idx];
    }

    template <typename T1>
    void push_back(T1&& value)
    {
        if (size() == capacity()) {
            realloc();
        }
        new (m_end) value_type(std::forward<T1>(value));
        ++m_end;
    }

    template <typename... Args>
    void emplace_back(Args&&... args)
    {
        if (size() == capacity()) {
            realloc();
        }
        new (m_end) value_type(std::forward<Args>(args)...);
        ++m_end;
    }

    iterator begin()
    {
        return iterator{m_start};
    }

    iterator end()
    {
        return iterator{m_end};
    }

  private:
    value_type* m_start;
    value_type* m_end;
    size_t m_capacity;
    Allocator alloc;

    void realloc()
    {
        m_capacity = (m_capacity == 0) ? 1 : m_capacity * 2;
        
        value_type* new_start = Alloc::allocate(alloc, m_capacity);

        std::ptrdiff_t sz = size();
        // Move construct existing stuff to new location
        for (std::ptrdiff_t idx = 0; idx < sz; ++idx) {
            new (new_start + idx) value_type(std::move(at(idx)));
            (m_start + idx)->~T();
        }

        // Delete older memory
        Alloc::deallocate(alloc, m_start, sz);

        // Update pointers
        m_start = new_start;
        m_end = m_start + sz;
    }

    // Destruct each element then deallocate memory
    void dealloc()
    {
        value_type* st = m_start;
        while (st != m_end) {
            st->~T();
            ++st;
        }
        Alloc::deallocate(alloc, m_start, size());
    }
};

}
