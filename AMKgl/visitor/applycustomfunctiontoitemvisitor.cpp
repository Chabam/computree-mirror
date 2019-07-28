#include "applycustomfunctiontoitemvisitor.h"

ApplyCustomFunctionToItemVisitor::ApplyCustomFunctionToItemVisitor()
{
    m_model = nullptr;
    m_scene = nullptr;
    m_setFunction = nullptr;
    m_continueVisitFunction = nullptr;
}

void ApplyCustomFunctionToItemVisitor::setFunction(ApplyCustomFunctionToItemVisitor::customFunction f)
{
    m_setFunction = f;
}

void ApplyCustomFunctionToItemVisitor::setContinueVisitFunction(ApplyCustomFunctionToItemVisitor::continueVisitFunction f)
{
    m_continueVisitFunction = f;
}

void ApplyCustomFunctionToItemVisitor::setCurrentInformation(const AMKgl::ItemModel *model, const PermanentItemScene *scene)
{
    m_model = (AMKgl::ItemModel*)model;
    m_scene = (PermanentItemScene*)scene;
}

AMKgl::ItemModel *ApplyCustomFunctionToItemVisitor::getCurrentModel() const
{
    return m_model;
}

PermanentItemScene *ApplyCustomFunctionToItemVisitor::getCurrentScene() const
{
    return m_scene;
}

bool ApplyCustomFunctionToItemVisitor::mustContinueVisit() const
{
    if(m_continueVisitFunction == nullptr)
        return true;

    return m_continueVisitFunction();
}

void ApplyCustomFunctionToItemVisitor::addItemDrawableToCompute(const AMKgl::Item *item)
{
    if(m_setFunction != nullptr)
        m_setFunction(item, m_model, m_scene);
}
