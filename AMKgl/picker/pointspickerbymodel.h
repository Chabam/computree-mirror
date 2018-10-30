#ifndef POINTSPICKERBYMODEL_H
#define POINTSPICKERBYMODEL_H

#include "genericpickerbymodel.h"
#include "pointspicker.h"
#include "visitor/iglobalpointsvisitorbymodel.h"

/**
 * @brief Interface of a picker for points (separated by model)
 */
class PointsPickerByModel : public GenericPickerByModel, public IGlobalPointsVisitorByModel
{
public:

    PointsPickerByModel();

    /**
     * @brief Set the current model and scene to use (called for each model)
     */
    void setCurrentInformation(const AMKgl::ItemModel* model,
                               const PermanentItemScene* scene);

    AMKgl::ItemModel* getCurrentModel() const { return GenericPickerByModel::getCurrentModel(); }
    PermanentItemScene* getCurrentScene() const { return GenericPickerByModel::getCurrentScene(); }

    /**
     * @brief Create (if not exist) and returns the points picker for the specified model
     */
    PointsPicker& createOrGetPointsPickerForModel(const ItemModel* model);

    /**
     * @brief Set the selection operation
     */
    void setSelectionOperation(PointsPicker::selectionOperation op);

    /**
     * @brief Returns the current operation
     */
    PointsPicker::selectionOperation getSelectionOperation() const;

    /**
     * @brief Set the operation that returns true if the point must be compute, false otherwise
     */
    void setCheckOperation(PointsPicker::checkOperation op);

    /**
     * @brief Returns the check operation
     */
    PointsPicker::checkOperation getCheckOperation() const;

    // -------- IGlobalPointsVisitorByModel ---------- //
    bool mustContinueVisit() const { return true; }
    void addPointsToCompute(const Eigen::Vector3d &offset, std::vector<GLuint> &indices, const size_t& firstPointGlobalIndex);

private:
    PointsPicker::checkOperation        m_checkOperation;
    PointsPicker::selectionOperation    m_selectionOperation;

protected:
    /**
     * @brief Initialize the picker after it was created
     */
    virtual void initPicker(GenericPicker* picker);
};

#endif // POINTSPICKERBYMODEL_H
