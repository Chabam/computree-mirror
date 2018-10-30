#include "performancestatisticmanager.h"

#include <QtAlgorithms>

PerformanceStatisticManager::PerformanceStatisticManager()
{
}

PerformanceStatisticManager::~PerformanceStatisticManager()
{
    qDeleteAll(m_stats.begin(), m_stats.end());
}

void PerformanceStatisticManager::startPerformanceStat(const int &index)
{
    m_stats[index]->restart();
}

void PerformanceStatisticManager::stopPerformanceStat(const int &index)
{
    m_stats[index]->elapsed();
}

void PerformanceStatisticManager::printAllStatistics()
{
    foreach (PerformanceStatistic *s, m_stats) {
        s->print();
    }
}

void PerformanceStatisticManager::resetAllStatistics()
{
    foreach (PerformanceStatistic *s, m_stats) {
        s->reset();
    }
}

void PerformanceStatisticManager::installOnePerformanceStat(const QString& name)
{
    m_stats.push_back(new PerformanceStatistic(name));
}
