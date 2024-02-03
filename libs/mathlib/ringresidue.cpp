#include "ringresidue.h"

RingResidue::RingResidue() : n(0), module(-1), type(ResidueType::Zn) {}
RingResidue::RingResidue(int n, int module, ResidueType type) : n(n), module(module), type(type) {}

int RingResidue::countOfGenerators(const char *operation) const
{
    switch (type)
    {
    case ResidueType::Zn:
        return Zn(n, module).countOfGenerators(operation);
    case ResidueType::Zp:
        return Zp(n, module).countOfGenerators(operation);
    case ResidueType::MultiGroup_Zn:
        return MultiGroup_Zn(n, module).countOfGenerators(operation);

    default:
        return -1;
    }
}

int RingResidue::pow(int deg) const
{
    return modulePower(n, deg, module);
}

int RingResidue::order(const char *operation) const
{
    if (not strcmp(operation, "+"))
    {
        Zn residue(n, module);
        switch (type)
        {
        case ResidueType::Zn:
        case ResidueType::Zp:
            if (n == 0) return 1;
            for (int i = 2; i <= residue.capacity(); ++i)
            {
                if (residue.capacity() % i != 0) continue;

                if (residue * Zn(i, module) == Zn(0, module))
                    return i;
            }
            return -2;

        default:
            return -1;
        }
    } else {
        if (type == ResidueType::Zn && not isPrime(module)) return -1;

        MultiGroup_Zn residue(n, module);
        if (n == 1) return 1;
        for (int i = 2; i <= residue.capacity(); ++i)
        {
            if (residue.capacity() % i != 0) continue;
            if (modulePower(n, i, module) == 1)
                return i;

        }
        return -2;
    }
}

int RingResidue::countOfAnswersLinear(int a, int b, int module)
{
    auto answers = solveLinear(a, b, module);

    if (answers[0] == -1)
        return 0;
    else
        return answers.size();
}

std::vector<int> RingResidue::solveLinear(int a, int b, int module)
{
    Zn A(a, module), B(b, module);
    int d = GCD(a, module);

    if (b % d != 0) return std::vector<int>({-1});

    else {
        std::vector<int> answers;
        for (int i = 0; i < module; ++i)
            if (A * Zn(i, module) == B)
                answers.push_back(i);
        return answers;
    }
}

int RingResidue::countOfAnswersPrimeQuadro(int a, int module)
{
    if (not isPrime(module))
    {
        std::cerr << "ERROR in reigresidue.cpp: module need be prime!" << std::endl;
        return -2;
    }

   SymbolLegandre symbol(a, module);
   if (symbol.solve() == 1) return 2;
   else if (symbol.solve() == -1) return 0;
   else return 1;
}

std::vector<int> RingResidue::solvePrimeQuadro(int a, int module)
{
    if (not isPrime(module))
    {
        std::cerr << "ERROR in reigresidue.cpp: module need be prime!" << std::endl;
        return std::vector<int>({-2});
    }

    if (countOfAnswersPrimeQuadro(a, module) == 0)
        return std::vector<int>({-1});

    Zn A(a, module);
    std::vector<int> answers;
    for (int i = 0; i < module; ++i)
        if (Zn(i, module) * Zn(i, module) == A)
            answers.push_back(i);
    return answers;
}

int RingResidue::countOfAnswersCompositeQuadro(int a, int module)
{
    auto answers = solveCompositeQuadro(a, module);

    if (answers[0] == -1)
        return 0;
    else
        return answers.size();
}

std::vector<int> RingResidue::solveCompositeQuadro(int a, int module)
{
    if (isPrime(module))
    {
        std::cerr << "ERROR in reigresidue.cpp: module cannot be prime!" << std::endl;
        return std::vector<int>({-2});
    }

    Zn A(a, module);
    std::vector<int> answers;
    for (int i = 0; i < module; ++i)
        if (Zn(i, module) * Zn(i, module) == A)
            answers.push_back(i);

    if (answers.empty())
        return std::vector<int>({-1});

    return answers;
}
