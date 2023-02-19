#include "dm_sortfiltermathproxymodel.h"

#include "dm_guimanager.h"

#include <QLocale>

#if defined(_UNICODE)
std::string wide_string_to_string(const std::wstring& wide_string)
{
    QString buf = QString::fromWCharArray(wide_string.c_str());
    return buf.toStdString();
}
#endif

DM_SortFilterMathProxyModel::DM_SortFilterMathProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    m_parser.SetDecSep(QLocale::system().decimalPoint().toLatin1());
    m_acceptRow = true;
}

void DM_SortFilterMathProxyModel::setVariableInMathExpression(const QString &var)
{
    m_var = var;
}

bool DM_SortFilterMathProxyModel::canSetMathExpression(const QString &expression, bool verbose)
{
    if(m_var.isEmpty())
        return false;

    if(expression.isEmpty())
        return true;

    if(!expression.contains(m_var))
        return false;

    QString tmp = expression;
    tmp.replace(m_var, "1");

    #if defined(_UNICODE)
    mu::string_type expression_buffer = wide_string_to_string(tmp.toStdWString());
    #else
    mu::string_type expression_buffer = tmp.toStdString();
    #endif

    try
    {
        // test expression
        m_parser.SetExpr(expression_buffer);
        m_parser.Eval();
    }
    catch(mu::Parser::exception_type &e)
    {
        QString msg = QString::fromStdString(e.GetMsg());

        if(verbose)
            GUI_LOG->addMessage(LogInterface::debug, LogInterface::gui, tr("Erreur dans l'expression mathématique : ") + msg);

        return false;
    }

    return true;
}

bool DM_SortFilterMathProxyModel::setMathExpression(const QString &expression)
{
    if(m_var.isEmpty())
        return false;

    if(expression.isEmpty())
    {
        m_mathExpression = expression;
        invalidateFilter();
        return true;
    }

    if(!canSetMathExpression(expression, true))
        return false;

    m_mathExpression = expression;

    QChar dPoint = QLocale::system().decimalPoint();
    m_mathExpression.replace('.', dPoint);
    m_mathExpression.replace(',', dPoint);

    invalidateFilter();

    return true;
}

QString DM_SortFilterMathProxyModel::variableInMathExpression() const
{
    return m_var;
}

void DM_SortFilterMathProxyModel::setAcceptRows(bool enable, bool invalidFilter)
{
    m_acceptRow = enable;

    if(invalidFilter)
        invalidateFilter();
}

bool DM_SortFilterMathProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(!m_acceptRow)
        return false;

    if(m_mathExpression.isEmpty())
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);

    QModelIndex source_index = sourceModel()->index(source_row, filterKeyColumn(), source_parent);

    if (!source_index.isValid())
        return true;

    QString key = sourceModel()->data(source_index, filterRole()).toString();

    QString tmp = m_mathExpression;
    tmp.replace(m_var, key);
    tmp.replace('.', QLocale::system().decimalPoint());
    tmp.replace(',', QLocale::system().decimalPoint());

    #if defined(_UNICODE)
    mu::string_type expression_buffer = wide_string_to_string(tmp.toStdWString());
    #else
    mu::string_type expression_buffer = tmp.toStdString();
    #endif

    try
    {
        // test expression
        m_parser.SetExpr(expression_buffer);
        bool v =  m_parser.Eval() != 0;
        return v;
    }
    catch(mu::Parser::exception_type &e)
    {
        QString msg = QString::fromStdString(e.GetMsg());

        GUI_LOG->addErrorMessage(LogInterface::gui, tr("Exception muParser : %1").arg(msg));
        return false;
    }

    return true;
}
