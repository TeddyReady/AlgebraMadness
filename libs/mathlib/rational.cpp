#include "rational.h"

Rational::Rational() : num(0), den(1) {}
Rational::Rational(int num) : num(num), den(1) {}
Rational::Rational(int num, int den) : num(num), den(den)
{
    simplify();
}

void Rational::simplify()
{
    int gcd = GCD(num, den);
    if (gcd != 1)
    { num /= gcd; den /= gcd; }

    if (den < 0) { this->num = -num; this->den = -den; }

    if (den == 0)
    {
        std::cerr << "ERROR in Rational.cpp: Denumenator cannot be zero!" << std::endl;
        this->den = 1;
    }
}

Rational Rational::operator- () const
{
    return Rational(-num, den);
}

Rational Rational::operator+ (const Rational &other) const
{
    int gcd = GCD(num, den);

    if (den == other.den)
        return Rational(num + other.num, den);

    return Rational(num * (other.den / gcd) + other.num * (den / gcd),
                    (den / gcd) * other.den);
}

void Rational::operator+= (const Rational &other)
{
    *this = *this + other;
}

Rational Rational::operator- (const Rational &other) const
{
    return *this + (-other);
}

void Rational::operator-= (const Rational &other)
{
    *this = *this - other;
}

Rational Rational::operator* (const Rational &other) const
{
    return Rational(num * other.num, den * other.den);
}

void Rational::operator*= (const Rational &other)
{
    *this = *this * other;
}

Rational Rational::operator/ (const Rational &other) const
{
    return *this * Rational(other.den, other.num);
}

void Rational::operator/= (const Rational &other)
{
    *this = *this / other;
}

bool Rational::operator== (const Rational &other) const
{
    return num * (other.den / GCD(den, other.den)) == other.num * (den / GCD(den, other.den));
}

bool Rational::operator!= (const Rational &other) const
{
    return !(*this == other);
}

bool Rational::operator< (const Rational &other) const
{
    return num * (other.den / GCD(den, other.den)) < other.num * (den / GCD(den, other.den));
}

bool Rational::operator<= (const Rational &other) const
{
    return (*this < other) || (*this == other);
}

bool Rational::operator> (const Rational &other) const
{
    return !(*this <= other);
}

bool Rational::operator>= (const Rational &other) const
{
    return !(*this < other);
}

Rational::operator int() const
{
    return static_cast<int>(num/den);
}

Rational::operator double() const
{
    return static_cast<double>(num/den);
}

Rational::operator std::string() const
{
    return std::to_string(num) + "/" + std::to_string(den);
}

std::ostream& operator<< (std::ostream& out, const Rational &other)
{
    return out << std::string(other);
}
