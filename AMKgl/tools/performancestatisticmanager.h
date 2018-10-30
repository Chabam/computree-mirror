#ifndef PERFORMANCESTATISTICMANAGER_H
#define PERFORMANCESTATISTICMANAGER_H

#include "performancestatistic.h"
#include <vector>

class PerformanceStatisticManager
{
public:
    PerformanceStatisticManager();
    virtual ~PerformanceStatisticManager();

    void printAllStatistics();

    void resetAllStatistics();

protected:
    void installOnePerformanceStat(const QString& name);

    void startPerformanceStat(const int& index);

    void stopPerformanceStat(const int& index);


private:
    std::vector<PerformanceStatistic*>  m_stats;
};

#endif // PERFORMANCESTATISTICMANAGER_H
