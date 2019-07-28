#include "pointspickerbymodel.h"

#include "scene/permanentitemscene.h"
#include "scene/permanentscene.h"

PointsPickerByModel::PointsPickerByModel() : GenericPickerByModel()
{
    m_checkOperation = nullptr;
    m_selectionOperation = nullptr;
}

void PointsPickerByModel::setCurrentInformation(const ItemModel *model, const PermanentItemScene *scene)
{
    GenericPickerByModel::setCurrentInformation(model, scene);

    PointsPicker* pPicker = static_cast<PointsPicker*>(lastPicker());
    pPicker->setGlobalPointCloudAttributesProvider(scene->getPermanentScene()->getPointCloudAttributesProvider());
}

PointsPicker& PointsPickerByModel::createOrGetPointsPickerForModel(const ItemModel *model)
{
    return static_cast<PointsPicker&>(createOrGetPickerForModel(model));
}

void PointsPickerByModel::setSelectionOperation(PointsPicker::selectionOperation op)
{
    m_selectionOperation = op;

    PickerCollectionIterator it(getPickers());

    while(it.hasNext())
        static_cast<PointsPicker*>(it.next().value())->setSelectionOperation(op);
}

PointsPicker::selectionOperation PointsPickerByModel::getSelectionOperation() const
{
    return m_selectionOperation;
}

void PointsPickerByModel::setCheckOperation(PointsPicker::checkOperation op)
{
    m_checkOperation = op;

    PickerCollectionIterator it(getPickers());

    while(it.hasNext())
        static_cast<PointsPicker*>(it.next().value())->setCheckOperation(op);
}

PointsPicker::checkOperation PointsPickerByModel::getCheckOperation() const
{
    return m_checkOperation;
}

void PointsPickerByModel::addPointsToCompute(const Eigen::Vector3d &offset, std::vector<GLuint> &indices, const size_t &firstPointGlobalIndex)
{
    PointsPicker* pPicker = static_cast<PointsPicker*>(lastPicker());

    if(pPicker != nullptr)
        pPicker->addPointsToCompute(offset, indices, firstPointGlobalIndex);
}

void PointsPickerByModel::initPicker(GenericPicker *picker)
{
    GenericPickerByModel::initPicker(picker);

    PointsPicker* pPicker = static_cast<PointsPicker*>(picker);

    pPicker->setSelectionOperation(getSelectionOperation());
    pPicker->setCheckOperation(getCheckOperation());
}
