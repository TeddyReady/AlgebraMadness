#include "eulerfunction.h"

EulerFunction::EulerFunction() : number(1) {}
EulerFunction::EulerFunction(int number) : number(number)
{
    if (number < 0)
    {
        std::cerr << "ERROR in eulerfunction.cpp: Number cannot be lower zero!" << std::endl;
        this->number = 1;
    }
}

int EulerFunction::solve() const 
{
    auto data = decompositionToSimple(number);
    int result = 1;

    for (size_t i = 0; i < data.size(); ++i)
        result *= fastPower(data[i].first, data[i].second) - fastPower(data[i].first, data[i].second - 1);

    return result;
}

void EulerFunction::set(int number) { this->number = number; }

int EulerFunction::get() const { return number; }
