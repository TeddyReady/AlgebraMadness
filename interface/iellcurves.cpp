#include "iellcurves.h"

EllCurvesInterface::EllCurvesInterface(std::pair<int, int> rangeA, std::pair<int, int> rangeB, std::pair<int, int> rangeP, EllCurvesOptions option)
    : rangeA(rangeA), rangeB(rangeB), rangeP(rangeP), option(option), gen(QRandomGenerator::global()) {}
EllCurvesInterface::~EllCurvesInterface() { gen = nullptr; }

void EllCurvesInterface::create()
{
    std::vector<std::pair<int, int>> data;
    bool isCorrect = false;

    switch (option)
    {
    case EllCurvesOptions::NextPoint:
        curve.set(gen->bounded(rangeA.first, rangeA.second), gen->bounded(rangeB.first, rangeB.second), gen->bounded(rangeP.first, rangeP.second));
        break;
    }
}

QString EllCurvesInterface::description()
{
    return QString("Вычислите группу точек эллпитической кривой");
}

QString EllCurvesInterface::task()
{
    return QString("a=" + QString::number(std::get<0>(curve.get())) + ' ' + ", b=" + QString::number(std::get<1>(curve.get())) + ' ' + ", GF(" + QString::number(std::get<2>(curve.get())) + ')' + ' ' + "  ~?");
}

QString EllCurvesInterface::answer()
{
    curve.FindPoints();
    std::vector<Point> ans = curve.solve();
    QString result = "";
    for (Point i : ans)
        result += QString('(' + QString::number(i.x) + ',' + QString::number(i.y) + ')' + ' ');
    curve.ClearPoints();
    return result;
}
