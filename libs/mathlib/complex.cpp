#include "complex.tpp"

template <>
Complex<double>::operator std::string() const
{
    std::string result;
    if (b > 0) {
        if (b == 1)
            result = to_string(a, 2) + "+i";
        else
            result = to_string(a, 2) + "+" + to_string(b, 2) + "i";
    }

    else if (b == 0)
        result = to_string(a, 2);

    else {
        if (b == -1)
            result = to_string(a, 2) + "-i";
        else
            result = to_string(a, 2) + to_string(b, 2) + "i";
    }

    return result;
}

template <>
Complex<int>::operator std::string() const
{
    std::string result;
    if (b > 0) {
        if (b == 1)
            result = std::to_string(a) + "+i";
        else
            result = std::to_string(a) + "+" + std::to_string(b) + "i";
    }

    else if (b == 0)
        result = std::to_string(a);

    else {
        if (b == -1)
            result = std::to_string(a) + "-i";
        else
            result = std::to_string(a) + std::to_string(b) + "i";
    }

    return result;
}
