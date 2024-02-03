#ifndef RING_OF_MEMBERS
#define RING_OF_MEMBERS
#include "basemath.h"
#include "complex.tpp"
#include "residue.h"

template <class T>
class RingOfMembers {
private:
    std::vector<T> members;
    unsigned int deg;

public:
    explicit RingOfMembers() : deg(0) {}
    explicit RingOfMembers(unsigned int deg, T value) : deg(deg) { members = std::vector<T>(deg, value); }
    explicit RingOfMembers(std::vector<T> vector) : members(vector), deg(members.size()) {}
    explicit RingOfMembers(std::initializer_list<T> list) : members(list), deg(members.size()) {}
    RingOfMembers(const RingOfMembers &other) : members(other.members), deg(other.deg) {}
    ~RingOfMembers() {}

    unsigned int getDeg() const { return deg - 1; }
    void simplify()
    {
        while (members[deg - 1] == static_cast<T>(0) && deg != 1)
        {
            --deg;
            members.pop_back();
        }
    }

    RingOfMembers operator -() const
    {
        RingOfMembers result(*this);

        for (unsigned int i = 0; i < result.deg; ++i)
            result.members[i] = -result.members[i];

        return result;
    }

    RingOfMembers<T> operator +(const RingOfMembers<T> &other) const
    {
        RingOfMembers<T> result(deg > other.deg ? other : *this);

        while (result.deg < std::max(deg, other.deg))
        {
            ++result.deg;
            result.members.push_back(static_cast<T>(0));
        }

        for (unsigned int i = 0; i < result.deg; ++i)
            result.members[i] += (deg > other.deg ? members[i] : other.members[i]);

        result.simplify();
        return result;
    }

    void operator +=(const RingOfMembers<T> &other)
    {
        *this = *this + other;
    }

    RingOfMembers<T> operator -(const RingOfMembers &other) const
    {
        return *this + (-other);
    }

    void operator -=(const RingOfMembers<T> &other)
    {
        *this = *this - other;
    }

    RingOfMembers<T> operator *(const RingOfMembers &other) const
    {
        RingOfMembers<T> result((deg + other.deg) - 1, static_cast<T>(0));

        for (unsigned int i = 0; i < deg; ++i)
            for (unsigned int j = 0; j < other.deg; ++j)
                result.members[i + j] += members[i] * other.members[j];

        result.simplify();
        return result;
    }

    RingOfMembers<T> operator *(const T &value) const
    {
        RingOfMembers<T> result(*this);

        for (unsigned int i = 0; i < deg; ++i)
            result.members[i] *= value;

        result.simplify();
        return result;
    }

    void operator *=(const RingOfMembers<T> &other)
    {
        *this = *this * other;
    }

    RingOfMembers<T> operator %(const RingOfMembers<T> &other) const
    {
        return *this - (*this / other) * other;
    }

    void operator %=(const RingOfMembers<T> &other)
    {
        *this = *this % other;
    }

    RingOfMembers<T> operator /(const RingOfMembers<T> &other) const
    {
        RingOfMembers<T> origin(*this), result({static_cast<T>(0)});
        RingOfMembers<T> tmp;

        do {
            tmp = RingOfMembers<T>::chainDevide(origin, other);
            result += tmp;
        } while (not tmp.isZero());

        result.simplify();
        return result;
    }

    void operator /=(const RingOfMembers<T> &other)
    {
        *this = *this / other;
    }

    bool operator ==(const RingOfMembers<T> &other) const
    {
        if (isZero() && other.isZero()) return true;

        return this->members == other.members;
    }

    bool operator !=(const RingOfMembers<T> &other) const
    {
        return not (this->members == other.members);
    }

    void operator /=(const RingOfMembers<T> &other) const
    {
        return not (*this == other);
    }

    friend RingOfMembers<T> GCD(const RingOfMembers<T> &B, const RingOfMembers<T> &A)
    {
        RingOfMembers<T> q, r, result, a = A, b = B;
        if (b.deg < a.deg) std::swap(a, b);

        if (RingOfMembers<T>(b % a).isZero())
            return a;

        do {
            q = b / a;
            r = b % a;
            if (not r.isZero())
                result = r;
            b = a;
            a = r;
        } while (not r.isZero());

        result.simplify();
        return result;
    }

    static RingOfMembers<T> chainDevide(RingOfMembers<T> &origin, const RingOfMembers<T> &devider)
    {
        if (origin.isZero())
            return RingOfMembers<T>({static_cast<T>(0)});

        origin.simplify();

        if (origin.deg < devider.deg)
            return RingOfMembers<T>({static_cast<T>(0)});

        else
        {
            RingOfMembers<T> result((origin.deg - devider.deg) + 1, static_cast<T>(0));
            result.members[result.deg - 1] = origin.members[origin.deg - 1] * RingOfMembers<T>::get_inverse(devider.members[devider.deg - 1]);

            origin -= devider * result;

            return result;
        }
    }

    bool isZero() const
    {
        for (unsigned int i = 0; i < deg; ++i)
            if (members[i] != static_cast<T>(0))
                return false;

        return true;
    }

    static T get_inverse(const T &value);

    std::string to_string(const T &value) const
    {
        return (std::string)value;
    }

    operator std::string() const
    {
        std::string result;

        for (unsigned int i = 1; i <= deg; ++i)
        {
            if (members[deg - i] == static_cast<T>(1))
                if (deg - i == 0)
                    result += std::to_string(1);
                else if (deg - i == 1)
                    result += "x";
                else
                    result += "x^{" + std::to_string(deg - i) + "}";

            else if (members[deg - i] == static_cast<T>(-1))
                if (deg - i == 0)
                    result += std::to_string(-1);
                else if (deg - i == 1)
                    result += "-x";
                else
                    result += "-x^{" + std::to_string(deg - i) + "}";

            else if (members[deg - i] == static_cast<T>(0))
                continue;

            else
                if (deg - i == 0)
                    result += to_string(members[deg - i]);
                else if (deg - i == 1)
                    result += to_string(members[deg - i]) + "x";
                else
                    result += to_string(members[deg - i]) + "x^{" + std::to_string(deg - i) + "}";

            if (i + 1 <= deg && members[deg - (i + 1)] >= static_cast<T>(0))
                result += "+";
        }
        if (result == "") return "0";

        return result;
    }

    friend std::ostream& operator<< (std::ostream &out, const RingOfMembers<T> &other)
    {
        return out << (std::string)other;
    }
};

template <> RingOfMembers<double> RingOfMembers<double>::chainDevide(RingOfMembers<double> &origin, const RingOfMembers<double> &devider);

template <> int RingOfMembers<int>::get_inverse(const int &value);
template <> double RingOfMembers<double>::get_inverse(const double &value);
template <> Zn RingOfMembers<Zn>::get_inverse(const Zn &value);
template <> Zp RingOfMembers<Zp>::get_inverse(const Zp &value);
template <> Complex<int> RingOfMembers<Complex<int>>::get_inverse(const Complex<int> &value);
template <> Complex<double> RingOfMembers<Complex<double>>::get_inverse(const Complex<double> &value);

template <> std::string RingOfMembers<int>::to_string(const int &value) const;
template <> std::string RingOfMembers<double>::to_string(const double &value) const;

#endif //RING_OF_MEMBERS
