#include "mebiusfunction.h"

MebiusFunction::MebiusFunction() : number(1) {}
MebiusFunction::MebiusFunction(int number) : number(number)
{
    if (number < 0)
    {
        std::cerr << "ERROR in mebiusfunction.cpp: Number cannot be lower zero!" << std::endl;
        this->number = 1;
    }
}

int MebiusFunction::solve() const 
{
    if (number == 1) return 1;
    else {
        auto data = decompositionToSimple(number);
        for (size_t i = 0; i < data.size(); ++i) {
            if (data[i].second > 1)
                return 0;
        }
        return fastPower(-1, data.size());
    }
}

void MebiusFunction::set(int number) { this->number = number; }

int MebiusFunction::get() const { return number; }
