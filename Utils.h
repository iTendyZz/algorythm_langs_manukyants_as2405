#pragma once
#include <iostream>
using namespace std;

#define INPUT_LINE(in, str) getline(in>>std::ws, str); \
std::cerr << str << std::endl

template <typename T>
T GetCorrectNumber(T min, T max)
{
    T x;
    while ((std::cin >> x).fail()
        || std::cin.peek() != '\n'
        || x < min || x > max)
    {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Type number ( " << min << " - " << max << "): ";
    }
    std::cerr << x << std::endl;
    return x;
}