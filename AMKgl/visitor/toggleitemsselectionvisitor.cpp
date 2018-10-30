#include "toggleitemsselectionvisitor.h"

#include "includeitem.h"

ToggleItemsSelectionVisitor::ToggleItemsSelectionVisitor()
{

}

void ToggleItemsSelectionVisitor::addItemDrawableToCompute(const AMKgl::Item *item)
{
    ((AMKgl::Item*)item)->setSelected(!item->isSelected());
}
