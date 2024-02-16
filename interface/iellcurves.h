#ifndef ELLCURVESINTERFACE_H
#define ELLCURVESINTERFACE_H
#include "interface.h"
#include "ellcurves.h"
#include "basemath.h"

enum class EllCurvesOptions {
    NextPoint
};

class EllCurvesInterface : public TaskInterface {
private:
    //int minNum, maxNum;
    std::pair<int, int> rangeA,rangeB, rangeP;
    EllCurvesOptions option;
    QRandomGenerator *gen;
    EllCurves curve;

public:
    explicit EllCurvesInterface(std::pair<int, int> rangeA,std::pair<int, int> rangeB, std::pair<int, int> rangeP, EllCurvesOptions option);
    ~EllCurvesInterface();

    void create();
    QString description();
    QString task();
    QString answer();
};

#endif // ELLCURVESINTERFACE_H
