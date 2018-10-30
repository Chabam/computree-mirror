#include "ct_lambdafuncobjectsvisitor.h"

CT_LambdaFuncObjectsVisitor::CT_LambdaFuncObjectsVisitor()
{
    m_continueF = nullptr;
    m_visitF = nullptr;
}

void CT_LambdaFuncObjectsVisitor::setContinueFunction(CT_LambdaFuncObjectsVisitor::continueFunction f)
{
    m_continueF = f;
}

void CT_LambdaFuncObjectsVisitor::setVisitFunction(CT_LambdaFuncObjectsVisitor::visitFunction f)
{
    m_visitF = f;
}

bool CT_LambdaFuncObjectsVisitor::continueVisit() const
{
    if(m_continueF == nullptr)
        return true;

    return m_continueF();
}

void CT_LambdaFuncObjectsVisitor::newObjectsAvailable()
{
    m_visitF(*this);
}
