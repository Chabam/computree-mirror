#ifndef ITEMCOLORIZER_H
#define ITEMCOLORIZER_H

#include "defineitem.h"
#include "includecolor.h"

#include "visitor/iobjectsvisitorbymodel.h"
#include "renderer/generic/generictypedef.h"

class PermanentItemSceneByModel;
class AnyElementAttributesProvider;
class PermanentItemInformation;

/**
 * @brief Use this object to set a color to an item in a scene.
 * @warning The Item must be in the scene otherwise it will not be colored
 */
class ItemColorizer : public IObjectsVisitorByModel
{
public:
    ItemColorizer();

    /**
     * @brief Set the scene to visit
     */
    void setSceneToVisit(const PermanentItemSceneByModel* scene);

    /**
     * @brief Set the color to apply
     */
    void setColor(const QColor& color);

    /**
     * @brief Apply the color to the item
     */
    void apply();

    /**
     * @brief Static method to apply a color to an item in a scene
     */
    static void staticSetColorToItem(const AMKgl::Item* item, const QColor& color, const PermanentItemSceneByModel* scene);

    // ------ IObjectsVisitorByModel ------ //
    void setModel(const AMKgl::ItemModel* model) { Q_UNUSED(model) }
    void setScene(const PermanentItemScene* scene);
    void setItem(const AMKgl::Item* item);

    // ----- IObjectsVisitor ----- //
    bool mustContinueVisit() const { return true; }
    void visitChunk(const IChunk* chunk);

private:
    PermanentItemSceneByModel*      m_sceneToVisit;
    PermanentItemScene*             m_scene;
    AMKgl::Item*                    m_item;
    Basic::LocalColor               m_color;
    PermanentItemInformation*       m_itemInfo;
};

#endif // ITEMCOLORIZER_H
