#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include "kg_vector.hpp"

struct S {
    int a;
    int b;
    S() : a{0}, b{0}
    {
        std::cout << "Default constructor called!" << std::endl;
    }

    S(int _a, int _b) : a{_a}, b{_b}
    {
        std::cout << "Parameterized constructor called!" << std::endl;
    }

    S(const S& other) : a{other.a}, b{other.b}
    {
        std::cout << "Copy constructor called!" << std::endl;
    }

    S& operator=(const S& other)
    {
        std::cout << "Copy-assignment constructor called!" << std::endl;
        if (this != &other) {
            a = other.a;
            b = other.b;
        }
        return *this;
    }

    S(S&& other) noexcept
        : a{std::move(other.a)}, b{std::move(other.b)}
    {
        std::cout << "Move constructor called!" << std::endl;
    }

    S& operator=(S&& other) noexcept
    {
        std::cout << "Move-assignment constructor called!" << std::endl;
        a = std::move(other.a);
        b = std::move(other.b);
        return *this;
    }

    ~S()
    {
        std::cout << "Destructor called!" << std::endl;
    }
};

int main()
{
    kg::vector<int> v = {10, 9, 8, 7, 6, 5};
    std::for_each(v.begin(), v.end(), [](auto& val) { val *= 2; });

    std::cout << "Contents of v before sort are (using range-for): ";
    for (const int& a : v) {
        std::cout << a << " ";
    }
    std::cout << std::endl;

    std::sort(v.begin(), v.end());

    std::cout << "Contents of v after sort are (using range-for): ";
    for (const int& a : v) {
        std::cout << a << " ";
    }
    std::cout << std::endl;

    try {
        std::cout << "v[6] = " << v.at(6) << std::endl;
    } catch (std::out_of_range& e) {
        std::cout << "Caught out_of_range: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Generic exception caught!" << std::endl;
    }

    kg::vector<std::string> v2{3, "str"};
    for (int i = 0; i < 2; ++i) {
        v2.push_back(std::to_string(i * 100));
    }

    kg::vector<std::string> vs{v2.begin(), v2.end()};
    vs[2] = "Two";
    vs[3] = std::string{"Three"};

    std::cout << "Contents of v2 are:" << std::endl;
    for (int i = 0; i < 5; ++i) {
        std::cout << "v2[" << i << "] = " << v2[i] << std::endl;
    }

    std::cout << "Contents of vs are:" << std::endl;
    for (int i = 0; i < 5; ++i) {
        std::cout << "vs[" << i << "] = " << vs[i] << std::endl;
    }

    std::cout << "Creating kg::vector<S> v3 and calling emplace_back 5 times" << std::endl;
    kg::vector<S> v3;
    for (int i = 0; i < 5; ++i) {
        v3.emplace_back(i*10, i*10 + 1);
    }

    std::cout << "Creating a new vector v4 and copy constructing it from v3" << std::endl;
    auto v4 = v3;
    std::cout << "Contents of v4 are:" << std::endl;
    for (int i = 0; i < 5; ++i) {
        std::cout << "v4[" << i << "] = {" << v4[i].a << ", " << v4[i].b << "}" << std::endl;
    }

    std::cout << "Creating a new vector v5 and move constructing it from v3" << std::endl;
    auto v5 = std::move(v3);
    std::cout << "Contents of v5 are:" << std::endl;
    for (int i = 0; i < 5; ++i) {
        std::cout << "v5[" << i << "] = {" << v5[i].a << ", " << v5[i].b << "}" << std::endl;
    }

    return 0;
}
