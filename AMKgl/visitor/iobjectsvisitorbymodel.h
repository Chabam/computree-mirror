#ifndef IOBJECTSVISITORBYMODEL_H
#define IOBJECTSVISITORBYMODEL_H

#include "defineitem.h"
#include "definemodel.h"

#include "iobjectsvisitor.h"

class PermanentItemScene;

/**
 * @brief This class can be used to visit any type of object in a scene that sort
 *        scene by models
 */
class IObjectsVisitorByModel : public IObjectsVisitor
{
public:
    virtual ~IObjectsVisitorByModel() {}

    /**
     * @brief Set the current model
     */
    virtual void setModel(const AMKgl::ItemModel* model) = 0;

    /**
     * @brief Set the current scene
     */
    virtual void setScene(const PermanentItemScene* scene) = 0;

    /**
     * @brief Set the current item
     */
    virtual void setItem(const AMKgl::Item* item) = 0;
};

#endif // IOBJECTSVISITORBYMODEL_H
