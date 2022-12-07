#ifndef TRANSPOSITIONGROUP_H
#define TRANSPOSITIONGROUP_H
#include <QRandomGenerator>
#include <QVector>
#include <QDebug>
#include <QMap>
#include "basemath.h"

enum class ViewMode {
    Standart, Cycle
};

class TranspositionGroup {
    QVector<QVector<int>> tp;
    ViewMode mode;
public:
    explicit TranspositionGroup();
    explicit TranspositionGroup(QVector<std::pair<int, int>>);
    TranspositionGroup operator *(TranspositionGroup &trans);
    TranspositionGroup operator ~();

    void setTask(int, ViewMode mode);
    int getTask();
    QVector<QVector<int>>& getTransposition();
    ViewMode getViewMode();
    QString writeToMode(ViewMode);
    QString cycleType();
    int getHaos();
    QString getEven();
    int getOrder();
};

#endif // TRANSPOSITIONGROUP_H
