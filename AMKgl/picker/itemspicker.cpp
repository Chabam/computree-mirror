#include "itemspicker.h"

ItemsPicker::ItemsPicker() : GenericPicker()
{
    m_selectionOperation = nullptr;
    m_checkOperation = nullptr;
    m_graphicsView = nullptr;
}

bool ItemsPicker::isValid() const
{
    return (GenericPicker::isValid()
                && (getSelectionOperation() != nullptr)
                && (getCheckOperation() != nullptr)
                && (getGraphicsView() != nullptr));
}

void ItemsPicker::setSelectionOperation(selectionOperation op)
{
    m_selectionOperation = op;
}

ItemsPicker::selectionOperation ItemsPicker::getSelectionOperation() const
{
    return m_selectionOperation;
}

void ItemsPicker::setCheckOperation(ItemsPicker::checkOperation op)
{
    m_checkOperation = op;
}

ItemsPicker::checkOperation ItemsPicker::getCheckOperation() const
{
    return m_checkOperation;
}

void ItemsPicker::setGraphicsView(const GraphicsView *view)
{
    m_graphicsView = (GraphicsView*)view;
}

GraphicsView* ItemsPicker::getGraphicsView() const
{
    return m_graphicsView;
}
