#include <iostream>
#include <string>
#include <algorithm>

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

    S(S&& other) : a{std::move(other.a)}, b{std::move(other.b)}
    {
        std::cout << "Move constructor called!" << std::endl;
    }

    S& operator=(S&& other)
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
    kg::vector<int> v;
    
    for (int i = 10; i >= 0; --i) {
        v.push_back(i);
    }

    std::cout << "Contents of v before sort are (using for-each): ";
    for (const int& a : v) {
        std::cout << a << " ";
    }
    std::cout << std::endl;

    std::sort(v.begin(), v.end());

    std::cout << "Contents of v after sort are (using for-each): ";
    for (const int& a : v) {
        std::cout << a << " ";
    }
    std::cout << std::endl;

    kg::vector<std::string> v2;
    for (int i = 0; i < 5; ++i) {
        v2.emplace_back(std::to_string(i * 100));
    }
    v2[2] = "Two";
    v2[3] = std::string{"Three"};

    std::cout << "Contents of v2 are:" << std::endl;
    for (int i = 0; i < 5; ++i) {
        std::cout << "v2[" << i << "] = " << v2[i] << std::endl;
    }

    std::cout << "Creating kg::vector<S> v3 and calling push_back 5 times" << std::endl;
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
