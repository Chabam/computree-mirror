#ifndef ITEMSSELECTEDVISITOR_H
#define ITEMSSELECTEDVISITOR_H

#include "visitor/iitemsvisitorbymodel.h"

#include <QList>

/**
 * @brief This visitor can help you to get selected/unselected items
 */
class ItemsSelectedVisitor : public IItemsVisitorByModel
{
public:
    ItemsSelectedVisitor();

    /**
     * @brief Set "true" if you want to keep selected Items, "false" to keep unselected
     */
    void setSelectionValue(bool e);

    /**
     * @brief Call to get founded items
     */
    const QList<AMKgl::Item*>& getFoundedItems() const;

    // -------- IItemsVisitorByModel ---------- //
    DECLARE_UNUSED_FUNC_IVISITORBYMODEL
    bool mustContinueVisit() const { return true; }
    void addItemDrawableToCompute(const AMKgl::Item* item);

private:
    bool                m_selectValue;
    QList<AMKgl::Item*> m_items;
};

#endif // ITEMSSELECTEDVISITOR_H
