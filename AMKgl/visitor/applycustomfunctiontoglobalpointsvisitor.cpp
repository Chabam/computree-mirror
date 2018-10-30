#include "applycustomfunctiontoglobalpointsvisitor.h"

ApplyCustomFunctionToGlobalPointsVisitor::ApplyCustomFunctionToGlobalPointsVisitor()
{
    m_setFunction = nullptr;
    m_continueVisitFunction = nullptr;
}

void ApplyCustomFunctionToGlobalPointsVisitor::setFunction(ApplyCustomFunctionToGlobalPointsVisitor::customFunction f)
{
    m_setFunction = f;
}

void ApplyCustomFunctionToGlobalPointsVisitor::setContinueVisitFunction(ApplyCustomFunctionToGlobalPointsVisitor::continueVisitFunction f)
{
    m_continueVisitFunction = f;
}

bool ApplyCustomFunctionToGlobalPointsVisitor::mustContinueVisit() const
{
    if(m_continueVisitFunction == nullptr)
        return true;

    return m_continueVisitFunction();
}

void ApplyCustomFunctionToGlobalPointsVisitor::addPointsToCompute(const Eigen::Vector3d &offset, std::vector<GLuint> &indices, const size_t &firstPointGlobalIndex)
{
    if(m_setFunction != nullptr)
        m_setFunction(getChunk(), offset, indices, firstPointGlobalIndex);
}
