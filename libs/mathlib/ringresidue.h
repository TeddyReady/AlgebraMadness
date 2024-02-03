#ifndef RINGRESIDUE_H
#define RINGRESIDUE_H
#include "symbollegandre.h"
#include "residue.h"

enum class ResidueType {
    None         ,
    Zn           ,
    Zp           ,
    MultiGroup_Zn,
};

class RingResidue {
private:
    int n, module;
    ResidueType type;

public:
    explicit RingResidue();
    explicit RingResidue(int n, int module, ResidueType type);

    int countOfGenerators(const char *operation) const;
    int pow(int deg) const;
    int order(const char *operation) const;

    static int countOfAnswersLinear(int a, int b, int module);
    static std::vector<int> solveLinear(int a, int b, int module);

    static int countOfAnswersPrimeQuadro(int a, int module);
    static std::vector<int> solvePrimeQuadro(int a, int module);

    static int countOfAnswersCompositeQuadro(int a, int module);
    static std::vector<int> solveCompositeQuadro(int a, int module);
};

#endif // RINGRESIDUE_H
