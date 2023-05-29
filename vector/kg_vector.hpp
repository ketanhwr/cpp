#pragma once

#include <memory>

namespace kg
{

template <typename T, typename Allocator = std::allocator<T>>
class vector
{
    using size_type = std::size_t;
    using value_type = T;
    using Alloc = std::allocator_traits<Allocator>;

  public:
    // Default constructor
    vector() : m_start{nullptr}, m_end{nullptr}, m_capacity{0} {}

    // Copy constructor
    vector(const vector& other);

    // Copy assignment constructor
    vector& operator=(const vector& other);

    // Move constructor
    vector(vector&& other);

    // Move assignment constructor
    vector& operator=(vector&& other);

    // Destructor
    ~vector()
    {
        // Destruct each element then deallocate memory
        value_type* st = m_start;
        while (st != m_end) {
            st->~T();
            ++st;
        }
        Alloc::deallocate(alloc, m_start, size());
    }

    // Current size
    size_type size()
    {
        return (m_end - m_start);
    }

    // Current capacity
    size_type capacity()
    {
        return m_capacity;
    }

    value_type& operator[](const size_t& idx)
    {
        return m_start[idx];
    }

    value_type& at(const size_t& idx)
    {
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

  private:
    value_type* m_start;
    value_type* m_end;
    size_t m_capacity;
    Allocator alloc;

    void realloc()
    {
        m_capacity = (m_capacity == 0) ? 1 : m_capacity * 2;
        
        value_type* new_start = Alloc::allocate(alloc, m_capacity);

        size_t sz = size();
        // Move construct existing stuff to new location
        for (std::ptrdiff_t idx = 0; idx < sz; ++idx) {
            new (new_start + idx) value_type(std::move(at(idx)));
        }

        // Delete older memory
        Alloc::deallocate(alloc, m_start, sz);

        // Update pointers
        m_start = new_start;
        m_end = m_start + sz;
    }
};

}
