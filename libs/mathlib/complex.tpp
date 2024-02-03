#ifndef COMPLEX_H
#define COMPLEX_H
#include "basemath.h"

/* Complex numbers can be Integers or Reals */
/*           T - int or double              */

template <class T>
class Complex {
public:
    T a, b;

    Complex() : a(0), b(0) {}
    Complex(T a) : a(a), b(0) {}
    Complex(T a, T b) : a(a), b(b) {}

    T norm() const
    {
        return a*a + b*b;
    }


    Complex<T> operator- () const
    {
        return Complex<T>(-a, -b);
    }


    Complex<T> operator+ (const Complex<T>& other) const
    {
        return Complex<T>(a + other.a, b + other.b);
    }


    void operator+= (const Complex<T>& other)
    {
        *this = *this + other;
    }


    Complex<T> operator- (const Complex<T>& other) const
    {
        return *this + (-other);
    }


    void operator-= (const Complex<T>& other)
    {
        *this = *this - other;
    }


    Complex<T> operator* (const Complex<T>& other) const
    {
        return Complex<T>(a * other.a - b * other.b, a * other.b + other.a * b);
    }


    void operator*= (const Complex<T>& other)
    {
        *this = *this * other;
    }

    Complex<T> operator/ (const Complex<T>& other) const
    {
        Complex<T> num = *this * Complex<T>(other.a, -other.b);
        double den = other.norm();

        return Complex<T>(num.a / den, num.b/den);
    }


    void operator/= (const Complex<T>& other)
    {
        *this = *this / other;
    }


    Complex<int> operator% (const Complex<int>& other) const
    {
        Complex<double> result = Complex<double>(a, b) / Complex<double>(other.a, other.b);
        return (*this - other*Complex<int>(round(result.a), round(result.b)));
    }

    void operator%= (const Complex<int>& other)
    {
        *this = *this % other;
    }

    bool operator== (const Complex<T> &other) const
    {
        return this->norm() == other.norm();
    }

    bool operator!= (const Complex<T> &other) const
    {
        return !(*this == other);
    }

    bool operator< (const Complex<T> &other)  const
    {
        return this->norm() < other.norm();
    }

    bool operator<= (const Complex<T> &other) const
    {
        return (*this < other) || (*this == other);
    }

    bool operator> (const Complex<T> &other) const
    {
        return !(*this <= other);
    }

    bool operator>= (const Complex<T> &other) const
    {
        return !(*this < other);
    }

    std::string to_string(double val, std::size_t prec) const
    {
        std::ostringstream os;
        os << std::fixed << std::setprecision(prec) << val;
        return os.str();
    }

    operator std::string() const;

    friend std::ostream& operator<< (std::ostream &out, const Complex<T> &other)
    {
        return out << std::string(other);
    }
};

template <> Complex<double>::operator std::string() const;
template <> Complex<int>::operator std::string() const;

#endif // COMPLEX_H
