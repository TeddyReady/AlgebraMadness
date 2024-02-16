#include "ellcurves.h"


int modInverse(int a, int m) {
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1;
}

EllCurves::EllCurves() : a(0), b(0), p(0){}

EllCurves::EllCurves(int a, int b, int p) : a(a), b(b), p(p) {
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
        int slope = (3 * P.x * P.x + a)* modInverse(2 * P.y, p) % p;
        int x3 = (slope * slope - 2 * P.x) % p;
        int y3 = (slope * (P.x - x3) - P.y) % p;
        return Point(x3, y3);
    }

    // обычное сложение
    int slope = (Q.y - P.y)* modInverse(Q.x - P.x, p) % p;
    int x3 = (slope * slope - P.x - Q.x) % p;
    int y3 = (slope * (P.x - x3) - P.y) % p;

    return Point(x3, y3);
}

void EllCurves::FindPoints() {
    for (int x = 1; x < p; ++x)
    {
        for (int y = 1; y < p; ++y) {
            if ((y * y) % p == (x * x * x + a * x + b) % p)
                points.push_back(Point(x, y));
        }
    }
}

std::vector<Point> EllCurves::solve(){
    /*Point Q = p1;
    do {
        Q = addPoints(Q, p1, a, p);
        if (!Q.isInfinity)
            points.push_back(Q);
    } while (!Q.isInfinity);*/

    return points;
}

void EllCurves::set(int a, int b, int p){
    if(a>p)
        this->a=a%p;
    else
        this->a=a;
    if(b>p)
        this->b=b%p;
    else
        this->b=b;
    this->p=p;
}


void EllCurves::ClearPoints(){
    points.clear();
}
std::tuple<int,int,int,std::vector<Point>> EllCurves::get()
{
    return std::tuple<int, int, int, std::vector<Point>>(a,b,p,points);
}

