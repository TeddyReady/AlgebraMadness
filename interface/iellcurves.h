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
    int minNum, maxNum;
    EllCurvesOptions option;
    QRandomGenerator *gen;
    EllCurves curve;

public:
    explicit EllCurvesInterface(int minNum, int maxNum, EllCurvesOptions option);
    ~EllCurvesInterface();

    void create();
    QString description();
    QString task();
    QString answer();
};

#endif // ELLCURVESINTERFACE_H
