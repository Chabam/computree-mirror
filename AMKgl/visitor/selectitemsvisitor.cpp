#include "selectitemsvisitor.h"

#include "includeitem.h"

SelectItemsVisitor::SelectItemsVisitor()
{
    m_selectValue = false;
}

void SelectItemsVisitor::setSelectionValue(bool e)
{
    m_selectValue = e;
}

void SelectItemsVisitor::addItemDrawableToCompute(const AMKgl::Item *item)
{
    ((AMKgl::Item*)item)->setSelected(m_selectValue);
}
