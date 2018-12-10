#include "ct_outabstractitemmodel.h"

CT_OutAbstractItemModel::CT_OutAbstractItemModel(const QString& displayableName) : SuperClass(displayableName)
{
}

CT_OutAbstractItemModel::CT_OutAbstractItemModel(const CT_OutAbstractItemModel& other) : SuperClass(other)
{
    IItemDrawableForModel* itemD = other.itemDrawable();

    if(itemD != NULL)
        setPrototype(itemD->itemToolForModel()->copyItem());
}

IItemDrawableForModel* CT_OutAbstractItemModel::itemDrawable() const
{
    return static_cast<IItemDrawableForModel*>(prototype());
}
