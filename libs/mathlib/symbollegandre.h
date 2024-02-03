#ifndef SYMBOLLEGANDRE_H
#define SYMBOLLEGANDRE_H
#include "basemath.h"

class SymbolLegandre {
protected:
    int a, p;

public:
    explicit SymbolLegandre();
    explicit SymbolLegandre(int a, int p);

    virtual int solve() const;
    void set(int a, int p);
    std::pair<int, int> get() const;

    friend std::ostream& operator<< (std::ostream& out, const SymbolLegandre &other);
};

class SymbolJacobi : public SymbolLegandre {
public:
    explicit SymbolJacobi();
    explicit SymbolJacobi(int a, int p);

    int solve() const override;
};

#endif // SYMBOLLEGANDRE_H
