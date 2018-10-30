#ifndef SELECTITEMSVISITOR_H
#define SELECTITEMSVISITOR_H

#include "visitor/iitemsvisitorbymodel.h"

#include <QtGlobal>

/**
 * @brief This visitor can help you to set selected/unselected all items
 */
class SelectItemsVisitor : public IItemsVisitorByModel
{
public:
    SelectItemsVisitor();

    /**
     * @brief Set "true" if you want to select Items, "false" to unselect it
     */
    void setSelectionValue(bool e);

    // -------- IItemsVisitorByModel ---------- //
    DECLARE_UNUSED_FUNC_IVISITORBYMODEL
    bool mustContinueVisit() const { return true; }
    void addItemDrawableToCompute(const AMKgl::Item* item);

private:
    bool    m_selectValue;
};

#endif // SELECTITEMSVISITOR_H
