#include "itemspickingdefaultfunction.h"

#include "includeitem.h"

void ItemsPickingDefaultFunction::staticSelectItem(Item *item,
                                                   PermanentItemInformation *info,
                                                   PermanentItemScene* scene,
                                                   AnyElementAttributesProvider* provider)
{
    Q_UNUSED(scene)
    Q_UNUSED(info)
    Q_UNUSED(provider)

    item->setSelected(true);
}

bool ItemsPickingDefaultFunction::staticCheckSelectItem(const Item* item,
                                                        const PermanentItemInformation *info,
                                                        const PermanentItemScene* scene,
                                                        const AnyElementAttributesProvider* provider)
{
    Q_UNUSED(info)
    Q_UNUSED(scene)
    Q_UNUSED(provider)

    return !item->isSelected();
}

void ItemsPickingDefaultFunction::staticUnselectItem(Item *item,
                                                     PermanentItemInformation *info,
                                                     PermanentItemScene* scene,
                                                     AnyElementAttributesProvider* provider)
{
    Q_UNUSED(scene)
    Q_UNUSED(info)
    Q_UNUSED(provider)

    item->setSelected(false);
}

bool ItemsPickingDefaultFunction::staticCheckUnselectItem(const Item* item,
                                                          const PermanentItemInformation *info,
                                                          const PermanentItemScene* scene,
                                                          const AnyElementAttributesProvider* provider)
{
    Q_UNUSED(info)
    Q_UNUSED(scene)
    Q_UNUSED(provider)

    return item->isSelected();
}

bool ItemsPickingDefaultFunction::staticIsItemSelected(const Item* item)
{
    return item->isSelected();
}
