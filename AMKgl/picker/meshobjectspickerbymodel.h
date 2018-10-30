#ifndef MESHOBJECTSPICKERBYMODEL_H
#define MESHOBJECTSPICKERBYMODEL_H

#include "genericpickerbymodel.h"
#include "meshobjectspicker.h"
#include "visitor/iobjectsvisitorbymodel.h"

/**
 * @brief Interface of a picker for objects of a mesh (separated by model)
 */
class MeshObjectsPickerByModel : public GenericPickerByModel, public IObjectsVisitorByModel
{
public:
    MeshObjectsPickerByModel(const quint8& nVertexPerObject);

    void setModel(const AMKgl::ItemModel* model);
    void setScene(const PermanentItemScene* scene);
    void setItem(const AMKgl::Item* item);

    AMKgl::ItemModel* getCurrentModel() const { return GenericPickerByModel::getCurrentModel(); }
    PermanentItemScene* getCurrentScene() const { return GenericPickerByModel::getCurrentScene(); }

    /**
     * @brief Create (if not exist) and returns the picker for the specified model
     */
    MeshObjectsPicker& createOrGetMeshObjectsPickerForModel(const ItemModel* model);

    /**
     * @brief Set the selection operation
     */
    void setSelectionOperation(MeshObjectsPicker::selectionOperation op);

    /**
     * @brief Returns the current operation
     */
    MeshObjectsPicker::selectionOperation getSelectionOperation() const;

    /**
     * @brief Set the operation that returns true if the point must be compute, false otherwise
     */
    void setCheckOperation(MeshObjectsPicker::checkOperation op);

    /**
     * @brief Returns the check operation
     */
    MeshObjectsPicker::checkOperation getCheckOperation() const;

    /**
     * @brief Returns the number of vertex per object to use
     */
    inline quint8 getNVertexPerObject() const { return m_nVertexPerObject; }

    // -------- IObjectsVisitor ---------- //
    bool mustContinueVisit() const { return true; }
    void visitChunk(const IChunk* chunk);

private:
    MeshObjectsPicker::checkOperation       m_checkOperation;
    MeshObjectsPicker::selectionOperation   m_selectionOperation;
    quint8                                  m_nVertexPerObject;
    ItemModel*                              m_model;

protected:
    /**
     * @brief Initialize the picker after it was created
     */
    virtual void initPicker(GenericPicker* picker);
};

#endif // MESHOBJECTSPICKERBYMODEL_H
