#ifndef ITEMSPICKER_H
#define ITEMSPICKER_H

#include "objectsofitempicker.h"
#include "genericpicker.h"
#include "visitor/iitemsvisitor.h"

#include "defineitem.h"

/**
 * @brief Interface of a picker for item
 */
class ItemsPicker : public GenericPicker, public IItemsVisitor
{
public:
    typedef ObjectsOfItemPicker::checkOperation     checkOperation;
    typedef ObjectsOfItemPicker::selectionOperation selectionOperation;

    ItemsPicker();

    /**
     * @brief Returns true if all elements was set and compute can be called
     */
    virtual bool isValid() const;

    /**
     * @brief Set the selection operation
     */
    void setSelectionOperation(selectionOperation op);

    /**
     * @brief Returns the current operation
     */
    selectionOperation getSelectionOperation() const;

    /**
     * @brief Set the operation that returns true if the point must be compute, false otherwise
     */
    void setCheckOperation(checkOperation op);

    /**
     * @brief Returns the check operation
     */
    checkOperation getCheckOperation() const;

    /**
     * @brief Set the graphics view to use when draw items
     */
    void setGraphicsView(const GraphicsView* view);

    /**
     * @brief Returns the graphics view used when draw items
     */
    GraphicsView* getGraphicsView() const;

private:
    selectionOperation  m_selectionOperation;
    checkOperation      m_checkOperation;
    GraphicsView*       m_graphicsView;
};

#endif // ITEMSPICKER_H
