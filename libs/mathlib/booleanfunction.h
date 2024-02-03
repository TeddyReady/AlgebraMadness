#ifndef BOOLEANFUNCTION_H
#define BOOLEANFUNCTION_H
#include "basemath.h"

class BooleanFunction {
private:
    std::vector<bool> func;
    int degree;

public:
    explicit BooleanFunction();
    explicit BooleanFunction(const std::vector<bool> &vec);
    explicit BooleanFunction(std::initializer_list<bool> list);

    std::string printPolinomial() const;
    bool isBelongsToClass_S() const;
    bool isBelongsToClass_L() const;
    bool isBelongsToClass_M() const;
    bool isBelongsToClass_T1() const;
    bool isBelongsToClass_T0() const;

    static bool isSystemFull(const std::vector<BooleanFunction> &system);

    operator std::string() const;
    friend std::ostream& operator<< (std::ostream& out, const BooleanFunction &other);
};

#endif // BOOLEANFUNCTION_H
