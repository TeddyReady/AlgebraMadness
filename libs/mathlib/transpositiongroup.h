#ifndef TRANSPOSITIONGROUP_H
#define TRANSPOSITIONGROUP_H
#include "basemath.h"

enum class ViewMode {
    Standart, Cycle,
     Decomposition,
    Neighbors, None
};

class TranspositionGroup {
private:
    std::vector<std::vector<int>> tp;

public:
    explicit TranspositionGroup();
    explicit TranspositionGroup(std::vector<std::pair<int, int>>);
    explicit TranspositionGroup(const std::string &str, int order);
    explicit TranspositionGroup(std::vector<int>, int);

    TranspositionGroup operator* (TranspositionGroup &trans);
    TranspositionGroup operator~ ();
    bool operator ==(const TranspositionGroup& trans);

    int getTask() const;
    std::vector<std::vector<int>>& getTransposition();
    std::string writeToMode(ViewMode, bool forTest = false) const;
    std::string cycleType();
    int getHaos();
    std::string getEven(bool forTest = false);
    int getOrder();
    TranspositionGroup simplify(int);
    void clear();

    operator std::string() const;
    friend std::ostream& operator<< (std::ostream& out, const TranspositionGroup &other);
};

#endif // TRANSPOSITIONGROUP_H
