#ifndef ELLCURVES_H
#define ELLCURVES_H

#include "basemath.h"

class Point {
public:
    int x, y;
    bool isInfinity;
    Point() : isInfinity(true) {}

    Point(int x, int y) : x(x), y(y), isInfinity(false) {}
};


class EllCurves {
private:
    int a, b, p;
    Point p1;
    std::vector<Point> points;
public:
    explicit EllCurves();
    explicit EllCurves(int a, int b, int p);

    Point addPoints(const Point& P, const Point& Q, int a, int p);
    void FindPoints();
    std::vector<Point> solve();
    void set(int a, int b, int p);
    std::tuple<int,int,int,std::vector<Point>> get();
    friend int modInverse(int a, int m);
    void ClearPoints();
};

#endif //ELLCURVES_H

