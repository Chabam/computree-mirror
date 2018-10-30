#ifndef GENERICPICKERBYMODEL_H
#define GENERICPICKERBYMODEL_H

#include <QObject>
#include <QHash>

#include "Eigen/Dense"

#include "definemodel.h"
#include "includemodel.h"
#include "visitor/ivisitorbymodel.h"

class AnyElementAttributesProvider;
class PermanentItemScene;
class GenericPicker;

using namespace AMKgl;

#define DECLARE_DEFAULT_FUNC_VISITORBYMODEL void setCurrentInformation(const AMKgl::ItemModel* model, const PermanentItemScene* scene) { GenericPickerByModel::setCurrentInformation(model, scene); } \
                                            AMKgl::ItemModel* getCurrentModel() const { return GenericPickerByModel::getCurrentModel(); } \
                                            PermanentItemScene* getCurrentScene() const { return GenericPickerByModel::getCurrentScene(); }

/**
 * @brief Abstract class for a picker (separate by model)
 */
class GenericPickerByModel : public QObject, public IVisitorByModel
{
    Q_OBJECT

public:
    typedef QHash<ItemModel*, GenericPicker*>           PickerCollection;
    typedef QHashIterator<ItemModel*, GenericPicker*>   PickerCollectionIterator;

    GenericPickerByModel();
    virtual ~GenericPickerByModel();

    /**
     * @brief Create (if not exist) and returns the picker for the specified model
     */
    GenericPicker& createOrGetPickerForModel(const ItemModel* model);

    /**
     * @brief Returns the picker for the specified model
     */
    GenericPicker* getPickerForModel(const ItemModel* model) const;

    /**
     * @brief Returns true if all elements was set and compute can be called
     */
    virtual bool isValid() const;

    /**
     * @brief Set the current model and scene to use (called for each model)
     */
    void setCurrentInformation(const AMKgl::ItemModel* model,
                               const PermanentItemScene* scene);

    /**
     * @brief Returns the current model
     */
    AMKgl::ItemModel* getCurrentModel() const;

    /**
     * @brief Returns the current scene
     */
    PermanentItemScene* getCurrentScene() const;

    /**
     * @brief Returns the provider used to enable/disable the selection of an object.
     */
    AnyElementAttributesProvider* getAnyElementsAttributesProvider(const ItemModel* model) const;

    /**
     * @brief Set the camera position
     */
    void setCameraPosition(const Eigen::Vector3d &position);

    /**
     * @brief Returns the camera position
     */
    const Eigen::Vector3d& getCameraPosition() const;

    /**
     * @brief Set the camera model view matrix
     */
    void setCameraModelViewMatrix(const Eigen::Matrix4d &mvMatrix);

    /**
     * @brief Returns the camera model view matrix
     */
    const Eigen::Matrix4d& getCameraModelViewMatrix() const;

    /**
     * @brief Returns pickers collection
     */
    const PickerCollection& getPickers() const;

public slots:

    /**
     * @brief Call this method to compute all
     */
    bool compute();

private:
    PickerCollection    m_pickers;
    GenericPicker*      m_lastPicker;
    Eigen::Vector3d     m_camPosition;
    Eigen::Matrix4d     m_camModelViewMatrix;
    AMKgl::ItemModel*   m_model;
    PermanentItemScene* m_scene;

protected:

    /**
     * @brief Create a new picker to use with a model
     */
    virtual GenericPicker* createNewPicker() = 0;

    /**
     * @brief Initialize the picker after it was created
     */
    virtual void initPicker(GenericPicker* picker);

    /**
     * @brief Returns the last picker created
     */
    GenericPicker* lastPicker() const;

signals:
    /**
     * @brief Emmited when picking has finished
     */
    void finished();
};

#endif // GENERICPICKERBYMODEL_H
