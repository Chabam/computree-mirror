#ifndef OBJECTSOFITEMPICKER_H
#define OBJECTSOFITEMPICKER_H

#include "objectsprocessorforpicker.h"
#include "scene/tools/permanentiteminformation.h"

#include "defineitem.h"

#include <vector>
#include <functional>

class PermanentItemScene;
class AnyElementAttributesProvider;

using namespace AMKgl;

/**
 * @brief Interface of a picker for objects
 */
class ObjectsOfItemPicker : public ObjectsProcessorForPicker
{
public:
    typedef PermanentItemInformation        ItemInfo;

    /**
     * @brief Check operation : must return true if we must check the item or not. It was called for each element
     *        that the item will draw. If the method returns false the compute will stop and return the current result.
     */
    typedef std::function<bool (const Item* item,
                                const ItemInfo* info,
                                const PermanentItemScene* scene,
                                const AnyElementAttributesProvider* provider)> checkOperation;

    /**
     * @brief Selection operation : called when one element of the item must be selected (because it was included in the selection shape)
     */
    typedef std::function<void (Item* item,
                                ItemInfo* info,
                                PermanentItemScene* scene,
                                AnyElementAttributesProvider* provider)> selectionOperation;

    ObjectsOfItemPicker();

    /**
     * @brief Returns true if all elements was set properly
     */
    virtual bool isValid() const;

    /**
     * @brief Set the item to compute
     * @param item : item to compute
     * @param scene : scene that contains this item
     */
    void setItem(const Item* item, const PermanentItemScene *scene);

    /**
     * @brief Returns the current computed item
     */
    Item* getItem() const;

    /**
     * @brief Returns the provider used to enable/disable the selection of an object.
     */
    AnyElementAttributesProvider* getAnyElementsAttributesProvider() const;

    /**
     * @brief Returns the current computed item information
     */
    ItemInfo* getItemInformation() const;

    /**
     * @brief Returns the scene of the current item
     */
    PermanentItemScene* getPermanentItemScene() const;

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
     * @brief Enable/Disable the check operation
     */
    void setCheckOperationEnabled(bool enable);

    /**
     * @brief Enable/Disable the selection operation
     */
    void setSelectionOperationEnabled(bool enable);

    /**
     * @brief Set the graphics view to use when draw items
     */
    void setGraphicsView(const GraphicsView* view);

    /**
     * @brief Returns the graphics view used when draw items
     */
    GraphicsView* getGraphicsView() const;

    /**
     * @brief Call this method to compute the item
     * @return true if at least one element draw by the item is in the selection
     */
    bool compute();

private:
    Item*                           m_item;
    ItemInfo*                       m_itemInfo;
    PermanentItemScene*             m_scene;
    selectionOperation              m_selectionOperation;
    checkOperation                  m_checkOperation;
    selectionOperation              m_selectionOperationOriginal;
    checkOperation                  m_checkOperationOriginal;
    GraphicsView*                   m_graphicsView;

    /**
     * @brief Dummy selection operation to disable it
     */
    static void dummySelectionOperation(Item* item,
                                        ItemInfo *info,
                                        PermanentItemScene *scene,
                                        AnyElementAttributesProvider* provider);


    /**
     * @brief Dummy check operation to disable it
     */
    static bool dummyCheckOperation(const Item* item,
                                    const ItemInfo *info,
                                    const PermanentItemScene* scene,
                                    const AnyElementAttributesProvider* provider);
};

#endif // OBJECTSOFITEMPICKER_H
