#pragma once

#include <exception>

namespace kg {

class bad_optional_acces : std::exception {
  public:
    const char* what() const override {
        return "bad_optional_access";
    }
};

template <typename T>
class optional {
  public:
    // Default constructor
    optional() : value{nullptr}
    { }

    ~optional() {
        if (has_value()) {
            delete m_value;
        }
    }

    bool has_value() const {
        return (m_value != nullptr);
    }

    operator bool() const {
        return has_value();
    }

    T* operator->() noexcept {
        return m_value;
    }

    const T* operator->() const noexcept {
        return m_value;
    }

    T& operator*() & noexcept {
        return *m_value;
    }

    const T& operator*() const& noexcept {
        return *m_value;
    }

    T&& operator*() && noexcept {
        return std::move(*m_value);
    }

    const T&& operator() const&& noexcept {
        return std::move(*m_value);
    }

    T& value() & {
        if (has_value()) {
            return *m_value;
        }

        throw kg::bad_optional_access("no value");
    }

    const T& value() const& {
        if (has_value()) {
            return *m_value;
        }

        throw kg::bad_optional_access("no value");
    }

    T&& value() && {
        if (has_value()) {
            return std::move(*m_value);
        }

        throw kg::bad_optional_access("no value");
    }

    const T&& value() const&& {
        if (has_value()) {
            return std::move(*m_value);
        }

        throw kg::bad_optional_access("no value");
    }

    template <typename U>
    T value_or(U&& default_value) const& {
        if (has_value()) {
            return *m_value;
        }
        return static_cast<T>(std::forward<U>(default_value));
    }

    template <typename U>
    T value_or(U&& default_value) && {
        if (has_value()) {
            return std::move(*m_value);
        }
        return static_cast<T>(std::forward<U>(default_value));
    }

  private:
    T* m_value;

    void dealloc()
};


}
