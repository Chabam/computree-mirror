#include "itemsselectedvisitor.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"

ItemsSelectedVisitor::ItemsSelectedVisitor()
{
    m_selectValue = false;
}

void ItemsSelectedVisitor::setSelectionValue(bool e)
{
    m_selectValue = e;
}

void ItemsSelectedVisitor::addItemDrawableToCompute(const AMKgl::Item *item)
{
    if(item->isSelected() == m_selectValue)
        m_items.append((AMKgl::Item*)item);
}

const QList<AMKgl::Item*>& ItemsSelectedVisitor::getFoundedItems() const
{
    return m_items;
}
