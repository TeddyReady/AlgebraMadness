#include "residue.h"

Zn::Zn(): n(0), module(1) {}
Zn::Zn(int n) : n(n), module(-1) {}
Zn::Zn(int n, int module): n(n), module(module)
{
    if (module == 0)
    {
        std::cerr << "ERROR in residue.cpp: Module cannot be zero!" << std::endl;
        this->module = 1;
    }

    simplify();
}

void Zn::simplify()
{
    if (module == -1)
        return;

    while (n >= module)
        n -= module;

    while (n < 0)
        n += module;
}

int Zn::capacity() const
{
    return module;
}

int Zn::countOfGenerators(const char *operation) const
{
    if (not strcmp(operation, "+"))
        return EulerFunction(module).solve();
    else if (not strcmp(operation, "*") && isPrime(module))
        return EulerFunction(module - 1).solve();
    else
        return -1;
}

void Zn::operator= (const Zn &other)
{
    n = other.n;
    module = other.module;
}

Zn Zn::operator- () const
{
    return Zn(-n, module);
}

Zn Zn::operator+ (const Zn &other) const
{
    if (module == -1 || other.module == -1)
    {
        if (module == -1 && other.module != -1)
            return Zn(n + other.n, other.module);
        else if (module != -1 && other.module == -1)
            return Zn(n + other.n, module);
        else
            return Zn(n + other.n);
    }


    if (module != other.module)
    {
        std::cerr << "ERROR in residue.cpp: Modules does be equal!" << std::endl;
        return *this;
    }

    return Zn(n + other.n, module);
}

void Zn::operator+= (const Zn &other)
{
    *this = *this + other;
}

Zn Zn::operator- (const Zn &other) const
{
    return *this + (-other);
}

void Zn::operator-= (const Zn &other)
{
    *this = *this - other;
}

Zn Zn::operator* (const Zn &other) const
{
    if (module == -1 || other.module == -1)
    {
        if (module == -1 && other.module != -1)
            return Zn(n * other.n, other.module);
        else if (module != -1 && other.module == -1)
            return Zn(n * other.n, module);
        else
            return Zn(n * other.n);
    }

    if (module != other.module)
    {
        std::cerr << "ERROR in residue.cpp: Modules does be equal!" << std::endl;
        return *this;
    }

    return Zn(n * other.n, module);
}

void Zn::operator*= (const Zn &other)
{
    *this = *this * other;
}

bool Zn::operator== (const Zn &other) const
{
    if (module == -1 || other.module == -1)
        return (n == other.n);

    if (module != other.module)
    {
        std::cerr << "ERROR in residue.cpp: Modules does be equal!" << std::endl;
        return false;
    }

    return (n == other.n);
}

bool Zn::operator!= (const Zn &other) const
{
    return !(*this == other);
}

bool Zn::operator< (const Zn &other) const
{
    if (module == -1 || other.module == -1)
        return (n < other.n);

    if (module != other.module)
    {
        std::cerr << "ERROR in residue.cpp: Modules does be equal!" << std::endl;
        return false;
    }
    return (n < other.n);
}

bool Zn::operator<= (const Zn &other) const
{
    return (*this < other || *this == other);
}

bool Zn::operator> (const Zn &other) const
{
    return !(*this <= other);
}

bool Zn::operator>= (const Zn &other) const
{
    return !(*this < other); 
}

Zn::operator std::string() const
{
    return std::to_string(n);
}

std::ostream& operator<< (std::ostream& out, const Zn &other)
{
    return out << std::string(other);
}

Zp::Zp() : Zn() {}
Zp::Zp(int n) : Zn(n) {}
Zp::Zp(int n, int module): Zn(n, module) 
{
    if (!isPrime(module))
    {
        std::cerr << "ERROR in residue.cpp: Zp capture only primes values!" << std::endl;
        this->n = 0;
        this->module = 1; 
    }
}
Zp::Zp(const Zn &other) : Zn(other) {}

void Zp::operator= (const Zn &other)
{
    n = other.n;
    module = other.module;
}

Zp Zp::operator~ () const
{
    for(int i = 0; i < module; ++i)
    {

        if (Zp(n * i, module).n == 1)
            return Zp(i, module);

    } return Zp(0, module);
}

Zp Zp::operator/ (const Zp &other) const
{
    Zn result(*this * (~other));
    return Zp(result.n, result.module);
}

void Zp::operator/= (const Zp &other)
{
    *this = *this / other;
}

MultiGroup_Zn::MultiGroup_Zn(): n(0), module(1) {}
MultiGroup_Zn::MultiGroup_Zn(int n) : n(n), module(-1) {}
MultiGroup_Zn::MultiGroup_Zn(int n, int module): n(n), module(module)
{
    if (this->module == 0)
    {
        std::cerr << "ERROR in residue.cpp: Module cannot be zero!" << std::endl;
        this->module = 1;
    }

    simplify();

    if (GCD(this->n, this->module) != 1 || n == 0)
    {
        std::cerr << "ERROR in residue.cpp: number cannot have dividers with module!" << std::endl;
        this->n = 1;
    }
}

void MultiGroup_Zn::simplify()
{
    if (module == -1)
        return;

    while (n >= module)
        n -= module;

    while (n < 0)
        n += module;
}

int MultiGroup_Zn::capacity() const
{
    return EulerFunction(module).solve();
}

int MultiGroup_Zn::get_module() const
{
    return module;
}

int MultiGroup_Zn::countOfGenerators(const char *operation) const
{
    if (not strcmp(operation, "*") && isPrime(module))
        return EulerFunction(module - 1).solve();
    else
        return -1;
}

void MultiGroup_Zn::operator= (const MultiGroup_Zn &other)
{
    n = other.n;
    module = other.module;
}

MultiGroup_Zn MultiGroup_Zn::operator~ () const
{
    for(int i = 1; i < module; ++i)
    {
        if (GCD(i, module) != 1) continue;
        else if (MultiGroup_Zn(n * i, module).n == 1)
            return MultiGroup_Zn(i, module);

    } return MultiGroup_Zn(1, module);
}

MultiGroup_Zn MultiGroup_Zn::operator* (const MultiGroup_Zn &other) const
{
    if (module == -1 || other.module == -1)
    {
        if (module == -1 && other.module != -1)
            return MultiGroup_Zn(n * other.n, other.module);
        else if (module != -1 && other.module == -1)
            return MultiGroup_Zn(n * other.n, module);
        else
            return MultiGroup_Zn(n * other.n);
    }

    if (module != other.module)
    {
        std::cerr << "ERROR in residue.cpp: Modules does be equal!" << std::endl;
        return *this;
    }

    return MultiGroup_Zn(n * other.n, module);
}

void MultiGroup_Zn::operator*= (const MultiGroup_Zn &other)
{
    *this = *this * other;
}

MultiGroup_Zn MultiGroup_Zn::operator/ (const MultiGroup_Zn &other) const
{
    return *this * (~other);
}

void MultiGroup_Zn::operator/= (const MultiGroup_Zn &other)
{
    *this = *this / other;
}

bool MultiGroup_Zn::operator== (const MultiGroup_Zn &other) const
{
    if (module == -1 || other.module == -1)
        return (n == other.n);

    if (module != other.module)
    {
        std::cerr << "ERROR in residue.cpp: Modules does be equal!" << std::endl;
        return false;
    }

    return (n == other.n);
}

bool MultiGroup_Zn::operator!= (const MultiGroup_Zn &other) const
{
    return !(*this == other);
}

bool MultiGroup_Zn::operator< (const MultiGroup_Zn &other) const
{
    if (module == -1 || other.module == -1)
        return (n < other.n);

    if (module != other.module)
    {
        std::cerr << "ERROR in residue.cpp: Modules does be equal!" << std::endl;
        return false;
    }
    return (n < other.n);
}

bool MultiGroup_Zn::operator<= (const MultiGroup_Zn &other) const
{
    return (*this < other || *this == other);
}

bool MultiGroup_Zn::operator> (const MultiGroup_Zn &other) const
{
    return !(*this <= other);
}

bool MultiGroup_Zn::operator>= (const MultiGroup_Zn &other) const
{
    return !(*this < other);
}

MultiGroup_Zn::operator std::string() const
{
    return std::to_string(n);
}

std::ostream& operator<< (std::ostream& out, const MultiGroup_Zn &other)
{
    return out << std::string(other);
}
