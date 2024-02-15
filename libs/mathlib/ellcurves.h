#ifndef ELLCURVES_H
#define ELLCURVES_H

#include <vector>
#include <tuple>
#include "basemath.h"

class Point {
public:
    int x, y;
    bool isInfinity;
    Point() : isInfinity(true) {}

    Point(int x, int y) : x(x), y(y), isInfinity(false) {}
};

/*int modInverse1(int a, int m) {
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1;
}*/

class EllCurves {
private:
    int a, b, p;
    Point p1;
    std::vector<Point> points;
public:
    explicit EllCurves();
    explicit EllCurves(int a, int b, int p, Point p1);

    Point addPoints(const Point& P, const Point& Q, int a, int p);
    std::vector<Point> solve();
    void set(int a, int b, int p, Point p1);
    std::tuple<int,int,int,std::vector<Point>> get();
};

#endif //ELLCURVES_H

