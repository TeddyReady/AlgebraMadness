#include "basemath.h"

std::vector<std::pair<int, int>> decompositionToSimple(int num)
{
    std::vector<std::pair<int, int>> deviders;
    if (isPrime(num))
         deviders.emplace_back(std::make_pair(num, 1));
    else {
        int count = 0;
        int localNum = num;
        for (size_t i = 2; i <= static_cast<size_t>(num / 2 + 1);) {
            if (localNum % i == 0){
                localNum /= i;
                count++;
            } else if (count != 0){
                deviders.emplace_back(std::make_pair(i, count));
                count = 0;
            } else i++;
        }
    }
    return deviders;
}

bool isPrime(int num)
{
    num = abs(num);

    if (num == 1)
        return true;

    if (num % 2 == 0)
        return false;

    for (int i = 3; i <= sqrt(num); i+= 2){
          if (num % i == 0) {
              return false;
          }
    }
    return true;
}

int GCD(int a, int b)
{
    a = abs(a); b = abs(b);
    return b ? GCD(b, a % b) : a;
}

int LCM(int a, int b)
{
    a = abs(a); b = abs(b);
    return a / GCD(a, b) * b;
}

long fastPower(long num, long deg) {
    long result = 1;
    while(deg) {
        if (deg % 2 == 0) {
            deg /= 2;
            num *= num;
        } else {
            deg--;
            result *= num;
        }
    } return result;
}

long modulePower(long num, long deg, int module) {
  if (deg == 0) return 1;
  int subNum = modulePower(num, deg / 2, module);
  if (deg % 2 == 0)
    return (subNum * subNum) % module;
  else
    return (num * subNum * subNum) % module;
}

int toMod(int num, int module)
{
    if (module <= 1) {
        std::cerr << "ERROR in basemath.cpp: Incorrect module!" << std::endl;
        return -1;
    }
    if (num < 0) {
        while (num < 0) num += module;
        return num;
    } else {
        while (num >= module) num -= module;
        return num;
    }
}

std::string to2(size_t dec, int format)
{
    std::string num;
    if (dec == 0)
    {
        while (format != num.size())
            num = "0" + num;

        return num;
    }

    int degree = findMax2Degree(dec);
    for (int i = degree; i >= 0; --i)
    {
        if (static_cast<int>(dec) - static_cast<int>(pow(2, i)) >= 0)
        {
            dec -= static_cast<size_t>(pow(2, i));
            num.append("1");
        }
        else num.append("0");
    }

    if (format == 0 || format < num.size())
        return num;
    else
    {
        while (format != num.size())
            num = "0" + num;

        return num;
    }
}

int findMax2Degree(size_t number)
{
    if (number == 0) return 0;

    int degree = 0;
    do {
        if (number % 2 == 1) number -= 1;
        if (number == 0) break;
        number /= 2;
        ++degree;
    } while (number != 0);
    return degree;
}

int countOfSymbol(const std::string &str, char symbol)
{
    int count = 0;
    for (size_t i = 0; i < str.size(); ++i)
        if (str.at(i) == symbol)
            ++count;

    return count;
}

