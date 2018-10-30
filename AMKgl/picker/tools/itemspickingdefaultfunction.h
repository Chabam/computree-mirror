#ifndef ITEMSPICKINGDEFAULTFUNCTION_H
#define ITEMSPICKINGDEFAULTFUNCTION_H

#include "scene/tools/permanentiteminformation.h"
#include "tools/anyelementattributesprovider.h"

#include "defineitem.h"

using namespace AMKgl;

class ItemsPickingDefaultFunction
{
public:
    /**
     * @brief Select the item
     */
    static void staticSelectItem(Item *item,
                                 PermanentItemInformation *info,
                                 PermanentItemScene* scene,
                                 AnyElementAttributesProvider* provider);

    /**
     * @brief Check if must test the item when select it
     */
    static bool staticCheckSelectItem(const Item* item,
                                      const PermanentItemInformation *info,
                                      const PermanentItemScene* scene,
                                      const AnyElementAttributesProvider* provider);

    /**
     * @brief Unselect the item
     */
    static void staticUnselectItem(Item* item,
                                   PermanentItemInformation *info,
                                   PermanentItemScene* scene,
                                   AnyElementAttributesProvider* provider);

    /**
     * @brief Check if must test the item when unselect it
     */
    static bool staticCheckUnselectItem(const Item* item,
                                        const PermanentItemInformation *info,
                                        const PermanentItemScene* scene,
                                        const AnyElementAttributesProvider* provider);

    /**
     * @brief Returns true if the item is selected
     */
    static bool staticIsItemSelected(const Item* item);
};

#endif // ITEMSPICKINGDEFAULTFUNCTION_H
