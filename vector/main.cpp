#include <iostream>

#include "kg_vector.hpp"

int main()
{
    kg::vector<int> v;
    
    for (int i = 0; i < 10; ++i) {
        v.push_back(i);
    }

    std::cout << "Contents of v are:" << std::endl;
    for (int i = 0; i < 10; ++i) {
        std::cout << "v[" << i << "] = " << v[i] << std::endl;
    }

    return 0;
}
