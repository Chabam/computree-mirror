#ifndef IVISITORBYMODEL_H
#define IVISITORBYMODEL_H

#include "definemodel.h"

#include <QtGlobal>

class PermanentItemScene;

#define DECLARE_UNUSED_FUNC_IVISITORBYMODEL void setCurrentInformation(const AMKgl::ItemModel* model, const PermanentItemScene* scene) { Q_UNUSED(model) Q_UNUSED(scene) } \
                                            AMKgl::ItemModel* getCurrentModel() const { return nullptr; } \
                                            PermanentItemScene* getCurrentScene() const { return nullptr; }


/**
 * @brief Interface for visit all Items/points/etc... from multiple scene sorted by Models
 */
class IVisitorByModel {
public:
    /**
     * @brief Set the current model and scene to use (called for each model)
     */
    virtual void setCurrentInformation(const AMKgl::ItemModel* model,
                                       const PermanentItemScene* scene) = 0;

    /**
     * @brief Returns the current model
     */
    virtual AMKgl::ItemModel* getCurrentModel() const = 0;

    /**
     * @brief Returns the current scene
     */
    virtual PermanentItemScene* getCurrentScene() const = 0;
};

#endif // IVISITORBYMODEL_H
