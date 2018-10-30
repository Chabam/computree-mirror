#include "meshobjectspickerbymodel.h"

#include "scene/permanentitemscene.h"
#include "scene/permanentscene.h"
#include "tools/edgecloudattributesprovider.h"
#include "tools/facecloudattributesprovider.h"

MeshObjectsPickerByModel::MeshObjectsPickerByModel(const quint8& nVertexPerObject)
{
    m_checkOperation = nullptr;
    m_selectionOperation = nullptr;
    m_nVertexPerObject = nVertexPerObject;
    m_model = NULL;
}

void MeshObjectsPickerByModel::setModel(const ItemModel *model)
{
    m_model = (ItemModel*)model;
}

void MeshObjectsPickerByModel::setScene(const PermanentItemScene *scene)
{
    setCurrentInformation(m_model, scene);

    MeshObjectsPicker* pPicker = static_cast<MeshObjectsPicker*>(lastPicker());
    pPicker->setGlobalPointCloudAttributesProvider(scene->getPermanentScene()->getPointCloudAttributesProvider());

    if(m_nVertexPerObject == 2)
        pPicker->setGlobalObjectCloudAttributesProvider(scene->getPermanentScene()->getEdgeCloudAttributesProvider());
    else if(m_nVertexPerObject == 3)
        pPicker->setGlobalObjectCloudAttributesProvider(scene->getPermanentScene()->getFaceCloudAttributesProvider());
}

void MeshObjectsPickerByModel::setItem(const Item *item)
{
    // TODO ?
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(item)
#endif
}

MeshObjectsPicker& MeshObjectsPickerByModel::createOrGetMeshObjectsPickerForModel(const ItemModel *model)
{
    return static_cast<MeshObjectsPicker&>(createOrGetPickerForModel(model));
}

void MeshObjectsPickerByModel::setSelectionOperation(MeshObjectsPicker::selectionOperation op)
{
    m_selectionOperation = op;

    PickerCollectionIterator it(getPickers());

    while(it.hasNext())
        static_cast<MeshObjectsPicker*>(it.next().value())->setSelectionOperation(op);
}

MeshObjectsPicker::selectionOperation MeshObjectsPickerByModel::getSelectionOperation() const
{
    return m_selectionOperation;
}

void MeshObjectsPickerByModel::setCheckOperation(MeshObjectsPicker::checkOperation op)
{
    m_checkOperation = op;

    PickerCollectionIterator it(getPickers());

    while(it.hasNext())
        static_cast<MeshObjectsPicker*>(it.next().value())->setCheckOperation(op);
}

MeshObjectsPicker::checkOperation MeshObjectsPickerByModel::getCheckOperation() const
{
    return m_checkOperation;
}

void MeshObjectsPickerByModel::visitChunk(const IChunk* chunk)
{
    MeshObjectsPicker* pPicker = static_cast<MeshObjectsPicker*>(lastPicker());

    if(pPicker != NULL)
        pPicker->visitChunk(chunk);
}

void MeshObjectsPickerByModel::initPicker(GenericPicker *picker)
{
    GenericPickerByModel::initPicker(picker);

    MeshObjectsPicker* pPicker = static_cast<MeshObjectsPicker*>(picker);

    pPicker->setSelectionOperation(getSelectionOperation());
    pPicker->setCheckOperation(getCheckOperation());
}
