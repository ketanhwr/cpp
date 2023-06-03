#pragma once

#include <exception>
#include <type_traits>
#include <new>

namespace kg {

class bad_optional_acces: public std::exception {
  public:
    const char* what() const override {
        return "bad_optional_access";
    }
};

struct nullopt_t {};

inline constexpr nullopt_t nullopt{};

template <typename T>
class optional {
  public:
    optional() : has_value{false}
    { }

    ~optional() {
        delloc();
    }

    bool has_value() const {
        return m_has_value;
    }

    operator bool() const {
        return m_has_value;
    }

    T* operator->() noexcept {
        return std::launder(reinterpret_cast<T*>(&m_value));
    }

    const T* operator->() const noexcept {
        return std::launder(reinterpret_cast<const T*>(&m_value));
    }

    T& operator*() & noexcept {
        return *std::launder(reinterpret_cast<T*>(&m_value));
    }

    const T& operator*() const& noexcept {
        return *std::launder(reinterpret_cast<const T*>(&m_value));
    }

    T&& operator*() && noexcept {
        return std::move(*std::launder(reinterpret_cast<T*>(&m_value)));
    }

    const T&& operator() const&& noexcept {
        return std::move(*std::launder(reinterpret_cast<const T*>(&m_value)));
    }

    T& value() & {
        if (has_value()) {
            return **this;
        }

        throw kg::bad_optional_access("no value");
    }

    const T& value() const& {
        if (has_value()) {
            return **this;
        }

        throw kg::bad_optional_access("no value");
    }

    T&& value() && {
        if (has_value()) {
            return std::move(**this);
        }

        throw kg::bad_optional_access("no value");
    }

    const T&& value() const&& {
        if (has_value()) {
            return std::move(**this);
        }

        throw kg::bad_optional_access("no value");
    }

    template <typename U>
    T value_or(U&& default_value) const& {
        if (has_value()) {
            return **this;
        }
        return static_cast<T>(std::forward<U>(default_value));
    }

    template <typename U>
    T value_or(U&& default_value) && {
        if (has_value()) {
            return std::move(**this);
        }
        return static_cast<T>(std::forward<U>(default_value));
    }

  private:
    std::aligned_storage_t<sizeof(T), alignof(T)> m_value;
    bool m_has_value;

    void dealloc() {
        if (has_value()) {
            std::launder(reinterpret_cast<T*>(&m_value))->~T();
        }
    }
};


}
