#ifndef PERFORMANCESTATISTIC_H
#define PERFORMANCESTATISTIC_H

#include <QElapsedTimer>
#include <QString>
#include <QDebug>

struct PerformanceStatistic {
    PerformanceStatistic(QString n) : name(n), sum(0), count(0) {}

    void restart() { timer.restart(); }
    void elapsed() { sum += timer.nsecsElapsed(); ++count; }
    void print() { qDebug() << QString("stat %1 %2 / count = %3 ; total = %4").arg(name).arg(((double)sum)/((double)count)).arg(count).arg(sum); }
    void reset() { sum = 0; count = 0; }

    QString         name;
    QElapsedTimer   timer;
    size_t          sum;
    size_t          count;
};

#endif // PERFORMANCESTATISTIC_H
