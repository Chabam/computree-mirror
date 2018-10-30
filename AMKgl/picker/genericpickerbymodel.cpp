#include "genericpickerbymodel.h"

#include "genericpicker.h"
#include "scene/permanentitemscene.h"

GenericPickerByModel::GenericPickerByModel()
{
    m_lastPicker = NULL;
    m_model = NULL;
    m_scene = NULL;
}

GenericPickerByModel::~GenericPickerByModel()
{
    qDeleteAll(m_pickers.begin(), m_pickers.end());
}

GenericPicker& GenericPickerByModel::createOrGetPickerForModel(const ItemModel *model)
{
    GenericPicker *picker = m_pickers.value((ItemModel*)model, NULL);

    if(picker == NULL) {
        picker = createNewPicker();

        initPicker(picker);

        m_pickers.insert((ItemModel*)model, picker);
    }

    return *picker;
}

GenericPicker* GenericPickerByModel::getPickerForModel(const ItemModel *model) const
{
    return m_pickers.value((ItemModel*)model, NULL);
}

bool GenericPickerByModel::isValid() const
{
    PickerCollectionIterator it(m_pickers);

    while(it.hasNext())
    {
        if(!it.next().value()->isValid())
            return false;
    }

    return true;
}

void GenericPickerByModel::setCurrentInformation(const ItemModel *model,
                                                 const PermanentItemScene* scene)
{
    m_model = (ItemModel*)model;
    m_scene = (PermanentItemScene*)scene;

    GenericPicker& picker = createOrGetPickerForModel(model);
    picker.setAnyElementsAttributesProvider(scene->getAnyElementsAttributesProvider());
    m_lastPicker = &picker;
}

AnyElementAttributesProvider* GenericPickerByModel::getAnyElementsAttributesProvider(const ItemModel *model) const
{
    GenericPicker* picker = getPickerForModel(model);
    return ((picker == NULL) ? NULL : picker->getAnyElementsAttributesProvider());
}

ItemModel *GenericPickerByModel::getCurrentModel() const
{
    return m_model;
}

PermanentItemScene *GenericPickerByModel::getCurrentScene() const
{
    return m_scene;
}

void GenericPickerByModel::setCameraPosition(const Eigen::Vector3d &position)
{
    m_camPosition = position;

    PickerCollectionIterator it(m_pickers);

    while(it.hasNext())
        it.next().value()->setCameraPosition(position);
}

const Eigen::Vector3d &GenericPickerByModel::getCameraPosition() const
{
    return m_camPosition;
}

void GenericPickerByModel::setCameraModelViewMatrix(const Eigen::Matrix4d &mvMatrix)
{
    m_camModelViewMatrix = mvMatrix;

    PickerCollectionIterator it(m_pickers);

    while(it.hasNext())
        it.next().value()->setCameraModelViewMatrix(mvMatrix);
}

const Eigen::Matrix4d &GenericPickerByModel::getCameraModelViewMatrix() const
{
    return m_camModelViewMatrix;
}

const GenericPickerByModel::PickerCollection &GenericPickerByModel::getPickers() const
{
    return m_pickers;
}

bool GenericPickerByModel::compute()
{
    if(!isValid()) {
        emit finished();
        return false;
    }

    bool ok = true;

    PickerCollectionIterator it(m_pickers);

    while(it.hasNext())
    {
        if(!it.next().value()->compute())
            ok = false;
    }

    emit finished();

    return ok;
}

void GenericPickerByModel::initPicker(GenericPicker* picker)
{
    picker->setCameraModelViewMatrix(getCameraModelViewMatrix());
    picker->setCameraPosition(getCameraPosition());
}

GenericPicker* GenericPickerByModel::lastPicker() const
{
    return m_lastPicker;
}
