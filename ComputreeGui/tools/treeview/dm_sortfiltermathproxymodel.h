#ifndef DM_SORTFILTERMATHPROXYMODEL_H
#define DM_SORTFILTERMATHPROXYMODEL_H

#include <QSortFilterProxyModel>

#if defined(_WIN32) && defined(_MSC_VER) // Microsoft Visual Studio Compiler
#elif (defined(__linux__) || defined(_WIN32)) && defined(__GNUC__) // GNU Compiler (gcc,g++) for Linux, Unix, and MinGW (Windows)
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#if __GNUC__ > 7
#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#else
#pragma GCC diagnostic ignored "-Wattributes" // See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=89325
#endif
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(__APPLE__) // Clang Compiler (Apple)
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#endif
#include "muParser/muParser.h"
#if defined(_WIN32) && defined(_MSC_VER)
#elif (defined(__linux__) || defined(_WIN32)) && defined(__GNUC__)
#pragma GCC diagnostic warning "-Wall"
#pragma GCC diagnostic warning "-Wextra"
#if __GNUC__ > 7
#pragma GCC diagnostic warning "-Wdeprecated-copy"
#else
#pragma GCC diagnostic warning "-Wattributes" // See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=89325
#endif
#pragma GCC diagnostic warning "-Wdeprecated-declarations"
#elif defined(__APPLE__)
#pragma GCC diagnostic warning "-Wall"
#pragma GCC diagnostic warning "-Wextra"
#pragma GCC diagnostic warning "-Wdeprecated-declarations"
#pragma GCC diagnostic warning "-Wint-in-bool-context"
#endif

class DM_SortFilterMathProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit DM_SortFilterMathProxyModel(QObject *parent = 0);

    void setVariableInMathExpression(const QString &var);
    bool canSetMathExpression(const QString &expression, bool verbose = false);
    bool setMathExpression(const QString &expression);

    QString variableInMathExpression() const;

    void setAcceptRows(bool enable, bool invalidFilter = true);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

private:
    mutable mu::Parser  m_parser;
    QString             m_mathExpression;
    QString             m_var;
    int                 m_acceptRow;
};

#endif // DM_SORTFILTERMATHPROXYMODEL_H
