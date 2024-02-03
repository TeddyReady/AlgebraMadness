#ifndef MEBIUSFUNCTION_H
#define MEBIUSFUNCTION_H
#include "basemath.h"

class MebiusFunction {
private:
    int number;

public:
    explicit MebiusFunction();
    explicit MebiusFunction(int number);

    int solve() const;
    void set(int number);
    int get()   const;
};

#endif // MEBIUSFUNCTION_H
