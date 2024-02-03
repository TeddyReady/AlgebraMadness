#ifndef EULERFUNCTION_H
#define EULERFUNCTION_H
#include "basemath.h"

class EulerFunction {
private:
    int number;

public:
    explicit EulerFunction();
    explicit EulerFunction(int number);

    int solve() const;
    void set(int number);
    int get()   const;
};

#endif // EULERFUNCTION_H
