#include "ellcurves.h"
#include <cmath>
#include <iostream>

EllCurves::EllCurves() : a(0), b(0), p(0), p1(Point()) {}

EllCurves::EllCurves(int a, int b, int p, Point p1) : a(a), b(b), p(p), p1(p1) {
    if (pow(p1.y, 2) != pow(p1.x, 3) + a * p1.x + b)
    {
        std::cerr << "ERROR in ellcurves.cpp: Point does not belong to the curve";
        this->p1 = Point();
        this->points.push_back(p1);
    }
    else
    {
        this->points.push_back(p1);
    }
}

Point EllCurves::addPoints(const Point& P, const Point& Q, int a, int p) {
    if (P.isInfinity) {
        return Q;
    }
    if (Q.isInfinity) {
        return P;
    }

    if (P.x == Q.x && P.y != Q.y) {
        return Point(); // результат - бесконечно удаленная точка
    }

    if (P.x == Q.x && P.y == Q.y) {
        // двойное сложение
        int slope = (3 * P.x * P.x + a) //* modInverse1(2 * P.y, p) % p;
        int x3 = (slope * slope - 2 * P.x) % p;
        int y3 = (slope * (P.x - x3) - P.y) % p;
        return Point(x3, y3);
    }

    // обычное сложение
    int slope = (Q.y - P.y)// * modInverse1(Q.x - P.x, p) % p;
    int x3 = (slope * slope - P.x - Q.x) % p;
    int y3 = (slope * (P.x - x3) - P.y) % p;

    return Point(x3, y3);
}
std::vector<Point> EllCurves::solve(){
    Point Q = p1;
    do {
        Q = addPoints(Q, p1, a, p);
        if (!Q.isInfinity)
            points.push_back(Q);
    } while (!Q.isInfinity);
    return points;
}

void EllCurves::set(int a, int b, int p, Point p1){
    this->a=a;
    this->b=b;
    this->p=p;
    this->p1=p1;
    this->points.push_back(p1);
}

std::tuple<int,int,int,int,std::vecotr<Point>> EllCurves::get(){
    std::tuple<int, int, int, std::vector<Point>> tup{a,b,p,points};
    return tup;
}

