#ifndef RATIONAL_H
#define RATIONAL_H
#include "basemath.h"

class Rational {
private:
    int num, den;

public:
    explicit Rational();
    explicit Rational(int num);
    explicit Rational(int num, int den);

    void simplify();

    Rational operator- ()                      const;
    Rational operator+ (const Rational &other) const;
    void operator+= (const Rational &other);
    Rational operator- (const Rational &other) const;
    void operator-= (const Rational &other);
    Rational operator* (const Rational &other) const;
    void operator*= (const Rational &other);
    Rational operator/ (const Rational &other) const;
    void operator/= (const Rational &other);

    bool operator== (const Rational &other) const;
    bool operator!= (const Rational &other) const;
    bool operator< (const Rational &other)  const;
    bool operator<= (const Rational &other) const;
    bool operator> (const Rational &other)  const;
    bool operator>= (const Rational &other) const;

    operator int()         const;
    operator double()      const;
    operator std::string() const;

    friend std::ostream& operator<< (std::ostream& out, const Rational &other);
};

#endif //RATIONAL_H
