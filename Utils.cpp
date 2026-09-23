#include "Utils.h"
#include <vector>
#include <algorithm>

double GetCorrectDiameter()
{
    std::vector<double> diameters = { 500, 700, 1000, 1400 };
    double x;
    while ((std::cin >> x).fail()
        || std::cin.peek() != '\n'
        || find(diameters.begin(), diameters.end(), x) == diameters.end())
    {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Type number (500, 700, 1000, 1400)";
    }
    std::cerr << x << std::endl;
    return x;
}