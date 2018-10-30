#ifndef TOGGLEITEMSSELECTIONVISITOR_H
#define TOGGLEITEMSSELECTIONVISITOR_H

#include "visitor/iitemsvisitorbymodel.h"

#include <QtGlobal>

/**
 * @brief This visitor can help you to set toggle the selection of all items
 */
class ToggleItemsSelectionVisitor : public IItemsVisitorByModel
{
public:
    ToggleItemsSelectionVisitor();

    // -------- IItemsVisitorByModel ---------- //
    DECLARE_UNUSED_FUNC_IVISITORBYMODEL
    bool mustContinueVisit() const { return true; }
    void addItemDrawableToCompute(const AMKgl::Item* item);
};

#endif // TOGGLEITEMSSELECTIONVISITOR_H
