#ifndef IITEMSVISITOR_H
#define IITEMSVISITOR_H

#include "defineitem.h"

class PermanentItemScene;

/**
 * @brief Interface for visit all Items from a scene that contains Items
 */
class IItemsVisitor {
public:
    virtual ~IItemsVisitor() {}

    /**
     * @brief Returns true if we must continue the visit, false otherwise
     */
    virtual bool mustContinueVisit() const = 0;

    /**
     * @brief Called when an item is available
     * @param item : item to test
     * @param scene : scene that contains this item
     */
    virtual void addItemDrawableToCompute(const AMKgl::Item* item) = 0;
};

#endif // IITEMSVISITOR_H
