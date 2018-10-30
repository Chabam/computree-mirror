#include "applycustomfunctiontoobjectvisitor.h"

ApplyCustomFunctionToObjectVisitor::ApplyCustomFunctionToObjectVisitor()
{
    m_continueF = nullptr;
    m_visitF = nullptr;
}

void ApplyCustomFunctionToObjectVisitor::setContinueFunction(ApplyCustomFunctionToObjectVisitor::continueFunction f)
{
    m_continueF = f;
}

void ApplyCustomFunctionToObjectVisitor::setVisitFunction(ApplyCustomFunctionToObjectVisitor::visitFunction f)
{
    m_visitF = f;
}

bool ApplyCustomFunctionToObjectVisitor::mustContinueVisit() const
{
    if(m_continueF == nullptr)
        return true;

    return m_continueF();
}

void ApplyCustomFunctionToObjectVisitor::visitChunk(const IChunk *chunk)
{
    m_visitF(chunk);
}
