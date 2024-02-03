#include "ringofmembers.tpp"

/* RingOfMembers::chainDevide */

template <>
RingOfMembers<double> RingOfMembers<double>::chainDevide(RingOfMembers<double> &origin, const RingOfMembers<double> &devider)
{
    if (origin.isZero())
        return RingOfMembers<double>({static_cast<double>(0)});

    origin.simplify();

    if (origin.deg < devider.deg)
        return RingOfMembers<double>({static_cast<double>(0)});

    else
    {
        RingOfMembers<double> result((origin.deg - devider.deg) + 1, static_cast<double>(0));

        result.members[result.deg - 1] =
                    origin.members[origin.deg - 1] * RingOfMembers<double>::get_inverse(devider.members[devider.deg - 1]);
//        );

        origin -= devider * result;

        return result;
    }
}

/* RingOfMembers::get_inverse */

template <>
int RingOfMembers<int>::get_inverse(__attribute__((unused)) const int &value)
{
    std::cerr << "ERROR in ringofmembers.cpp: Cannot find inverse of integer variable!" << std::endl;
    return 0;
}

template <>
double RingOfMembers<double>::get_inverse(const double &value)
{
    if (value == 0)
    {
        std::cerr << "ERROR in ringofmembers.cpp: Zero cannot have an inverse!" << std::endl;
        return 0;
    }

    return 1/value;
}

template <>
Zn RingOfMembers<Zn>::get_inverse(const Zn &value)
{
    if (value == static_cast<Zn>(0))
    {
        std::cerr << "ERROR in ringofmembers.cpp: Zero cannot have an inverse!" << std::endl;
        return Zn(0);
    }

    else if (not isPrime(value.capacity()))
    {
        std::cerr << "ERROR in ringofmembers.cpp: Cannot find inverse of Zn ring variable!" << std::endl;
        return Zn(0);
    }

    for (int i = 0; i < value.capacity(); ++i)
    {
        if (Zn(i) * value == Zn(1))
            return Zn(i);
    }

    std::cerr << "ERROR in ringofmembers.cpp: Failed finding inverse of Zp variable!" << std::endl;
    return Zn(0);
}

template <>
Zp RingOfMembers<Zp>::get_inverse(const Zp &value)
{
    return RingOfMembers<Zn>::get_inverse(value);
}

template <>
Complex<int> RingOfMembers<Complex<int>>::get_inverse(__attribute__((unused)) const Complex<int> &value)
{
    std::cerr << "ERROR in ringofmembers.cpp: Cannot find inverse of gaussian integer variable!" << std::endl;
    return Complex<int>(0);
}

template <>
Complex<double> RingOfMembers<Complex<double>>::get_inverse(const Complex<double> &value)
{
    return static_cast<Complex<double>>(1) / value;
}

/* RingOfMembers::to_string */

template <>
std::string RingOfMembers<int>::to_string(const int &value) const
{
    return std::to_string(value);
}

template <>
std::string RingOfMembers<double>::to_string(const double &value) const
{
    return std::to_string(value);
}
