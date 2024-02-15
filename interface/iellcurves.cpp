#include "iellcurves.h"

EllCurvesInterface::EllCurvesInterface(int minNum, int maxNum, EllCurvesOptions option)
    : minNum(minNum), maxNum(maxNum), option(option), gen(QRandomGenerator::global()) {}
EllCurvesInterface::~EllCurvesInterface() { gen = nullptr; }

void EllCurvesInterface::create()
{
    std::vector<std::pair<int, int>> data;
    bool isCorrect = false;

    switch (option)
    {
    case EllCurvesOptions::NextPoint:
        Point p1(gen->bounded(minNum, maxNum),gen->bounded(minNum, maxNum));
        curve.set(gen->bounded(minNum, maxNum),gen->bounded(minNum, maxNum),gen->bounded(minNum, maxNum),p1);
        break;
    }
}

QString EllCurvesInterface::description()
{
    return QString("Вычислите группу точек эллпитической кривой");
}

QString EllCurvesInterface::task()
{
    // Не забывай про ?, вместо него подставляется ответ
    return QString("hello ?");
}

QString EllCurvesInterface::answer()
{
    // Крашится программа здесь!
//    std::vector<Point> ans = curve.solve();
//    QString result;
//    for (Point i : ans)
//    {
//        std::cout << i.x << ' ' << i.y << '\n';
//        result += QString::number(i.x) + ',' + QString::number(i.y) +'\n';
//    }
//    return result;

    return QString("ToDo");
}
