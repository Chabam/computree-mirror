#ifndef APPLYCUSTOMFUNCTIONTOITEMVISITOR_H
#define APPLYCUSTOMFUNCTIONTOITEMVISITOR_H

#include "visitor/iitemsvisitorbymodel.h"

#include <functional>

/**
 * @brief This visitor can help you to apply a custom function to all items in the view
 */
class ApplyCustomFunctionToItemVisitor : public IItemsVisitorByModel
{
public:
    /**
     * Custom function to apply to items.
     */
    typedef std::function<void (const AMKgl::Item* item,
                                const AMKgl::ItemModel* model,
                                const PermanentItemScene* scene)> customFunction;

    /**
     * Custom function to know if we must continue or not the visit.
     */
    typedef std::function<bool ()> continueVisitFunction;

    ApplyCustomFunctionToItemVisitor();

    /**
     * @brief Call this method and set the your custom function.
     * @example If i want to select all items :
     *
     *          ApplyCustomFunctionToItemVisitor visitor;
     *          visitor.setFunction([](const AMKgl::Item* item,
     *                                 const AMKgl::ItemModel* model,
     *                                 const PermanentItemScene* scene) {
     *              ((AMKgl::Item*)item)->setSelected(true);
     *          } );
     *
     *          scene.visitItems(visitor);
     */
    void setFunction(customFunction f);

    /**
     * @brief Call this method to set the function that say if we must continue or not the visit
     */
    void setContinueVisitFunction(continueVisitFunction f);

    // -------- IItemsVisitorByModel ---------- //
    void setCurrentInformation(const AMKgl::ItemModel* model, const PermanentItemScene* scene);
    AMKgl::ItemModel* getCurrentModel() const;
    PermanentItemScene* getCurrentScene() const;
    bool mustContinueVisit() const;
    void addItemDrawableToCompute(const AMKgl::Item* item);

private:
    AMKgl::ItemModel*               m_model;
    PermanentItemScene*             m_scene;
    customFunction                  m_setFunction;
    continueVisitFunction           m_continueVisitFunction;
};

#endif // APPLYCUSTOMFUNCTIONTOITEMVISITOR_H
