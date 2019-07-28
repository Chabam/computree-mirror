#include "itemspickerbymodel.h"

ItemsPickerByModel::ItemsPickerByModel() : GenericPickerByModel()
{
    m_selectionOperation = nullptr;
    m_checkOperation = nullptr;
    m_graphicsView = nullptr;
}

ItemsPicker& ItemsPickerByModel::createOrGetItemsPickerForModel(const ItemModel *model)
{
    return static_cast<ItemsPicker&>(createOrGetPickerForModel(model));
}

void ItemsPickerByModel::setSelectionOperation(selectionOperation op)
{
    m_selectionOperation = op;

    PickerCollectionIterator it(getPickers());

    while(it.hasNext())
        static_cast<ItemsPicker*>(it.next().value())->setSelectionOperation(op);
}

ItemsPickerByModel::selectionOperation ItemsPickerByModel::getSelectionOperation() const
{
    return m_selectionOperation;
}

void ItemsPickerByModel::setCheckOperation(ItemsPickerByModel::checkOperation op)
{
    m_checkOperation = op;

    PickerCollectionIterator it(getPickers());

    while(it.hasNext())
        static_cast<ItemsPicker*>(it.next().value())->setCheckOperation(op);
}

ItemsPickerByModel::checkOperation ItemsPickerByModel::getCheckOperation() const
{
    return m_checkOperation;
}

void ItemsPickerByModel::addItemDrawableToCompute(const Item *item)
{
    ItemsPicker* pPicker = static_cast<ItemsPicker*>(lastPicker());

    if(pPicker != nullptr)
        pPicker->addItemDrawableToCompute(item);
}

void ItemsPickerByModel::setGraphicsView(const GraphicsView *view)
{
    m_graphicsView = (GraphicsView*)view;

    PickerCollectionIterator it(getPickers());

    while(it.hasNext())
        static_cast<ItemsPicker*>(it.next().value())->setGraphicsView(view);
}

GraphicsView* ItemsPickerByModel::getGraphicsView() const
{
    return m_graphicsView;
}

void ItemsPickerByModel::initPicker(GenericPicker *picker)
{
    GenericPickerByModel::initPicker(picker);

    ItemsPicker* iPicker = static_cast<ItemsPicker*>(picker);

    iPicker->setSelectionOperation(getSelectionOperation());
    iPicker->setCheckOperation(getCheckOperation());
    iPicker->setGraphicsView(getGraphicsView());
}
