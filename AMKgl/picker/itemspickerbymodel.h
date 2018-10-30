#ifndef ITEMSPICKERBYMODEL_H
#define ITEMSPICKERBYMODEL_H

#include "genericpickerbymodel.h"
#include "itemspicker.h"
#include "visitor/iitemsvisitorbymodel.h"

class ItemsPickerByModel : public GenericPickerByModel, public IItemsVisitorByModel
{
public:
    typedef ItemsPicker::checkOperation checkOperation;
    typedef ItemsPicker::selectionOperation selectionOperation;

    ItemsPickerByModel();

    DECLARE_DEFAULT_FUNC_VISITORBYMODEL

    /**
     * @brief Create (if not exist) and returns the items picker for the specified model
     */
    ItemsPicker& createOrGetItemsPickerForModel(const ItemModel* model);

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
    void setGraphicsView(const GraphicsViewInterface* view);

    /**
     * @brief Returns the graphics view used when draw items
     */
    GraphicsViewInterface* getGraphicsView() const;

    // -------- IItemsVisitorByModel ---------- //
    bool mustContinueVisit() const { return true; }
    void addItemDrawableToCompute(const AMKgl::Item* item);

private:
    selectionOperation              m_selectionOperation;
    checkOperation                  m_checkOperation;
    GraphicsViewInterface*          m_graphicsView;

protected:

    /**
     * @brief Initialize the picker after it was created
     */
    void initPicker(GenericPicker* picker);
};

#endif // ITEMSPICKERBYMODEL_H
