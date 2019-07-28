#include "objectsofitempicker.h"

#include "defineitempickingpainter.h"
#include "includeitempickingpainter.h"
#include "includeitem.h"
#include "scene/permanentitemscene.h"

ObjectsOfItemPicker::ObjectsOfItemPicker() : ObjectsProcessorForPicker()
{
    m_item = nullptr;
    m_itemInfo = nullptr;
    m_scene = nullptr;
    m_selectionOperation = nullptr;
    m_selectionOperationOriginal = nullptr;
    m_checkOperation = nullptr;
    m_checkOperationOriginal = nullptr;
    m_graphicsView = nullptr;

    setSelectionOperationEnabled(false);
    setCheckOperationEnabled(false);
}

bool ObjectsOfItemPicker::isValid() const
{
    return (m_item != nullptr)
            && (m_itemInfo != nullptr)
            && (m_scene != nullptr)
            && (m_selectionOperation != nullptr)
            && (m_checkOperation != nullptr)
            && (getAnyElementsAttributesProvider() != nullptr)
            && (getGraphicsView() != nullptr);
}

void ObjectsOfItemPicker::setItem(const Item *item, const PermanentItemScene* scene)
{
    m_item = (Item*)item;
    m_scene = (PermanentItemScene*)scene;
    m_itemInfo = m_scene->getPermanentItemInformation(item);
}

Item* ObjectsOfItemPicker::getItem() const
{
    return m_item;
}

AnyElementAttributesProvider* ObjectsOfItemPicker::getAnyElementsAttributesProvider() const
{
    return m_scene == nullptr ? nullptr : m_scene->getAnyElementsAttributesProvider();
}

ObjectsOfItemPicker::ItemInfo* ObjectsOfItemPicker::getItemInformation() const
{
    return m_itemInfo;
}

PermanentItemScene* ObjectsOfItemPicker::getPermanentItemScene() const
{
    return m_scene;
}

void ObjectsOfItemPicker::setSelectionOperation(ObjectsOfItemPicker::selectionOperation op)
{
    m_selectionOperation = op;
    m_selectionOperationOriginal = op;
}

ObjectsOfItemPicker::selectionOperation ObjectsOfItemPicker::getSelectionOperation() const
{
    return m_selectionOperation;
}

void ObjectsOfItemPicker::setCheckOperation(ObjectsOfItemPicker::checkOperation op)
{
    m_checkOperation = op;
}

ObjectsOfItemPicker::checkOperation ObjectsOfItemPicker::getCheckOperation() const
{
    return m_checkOperation;
}

void ObjectsOfItemPicker::setCheckOperationEnabled(bool enable)
{
    if(enable)
        m_checkOperation = m_checkOperationOriginal;
    else
        m_checkOperation = &ObjectsOfItemPicker::dummyCheckOperation;
}

void ObjectsOfItemPicker::setSelectionOperationEnabled(bool enable)
{
    if(enable)
        m_selectionOperation = m_selectionOperationOriginal;
    else
        m_selectionOperation = &ObjectsOfItemPicker::dummySelectionOperation;
}

void ObjectsOfItemPicker::setGraphicsView(const GraphicsView *view)
{
    m_graphicsView = (GraphicsView*)view;
}

GraphicsView *ObjectsOfItemPicker::getGraphicsView() const
{
    return m_graphicsView;
}

bool ObjectsOfItemPicker::compute()
{
    if(!isValid()) {
        emit finished();
        return false;
    }

    bool selected = false;

    if(getCheckOperation()(getItem(), getItemInformation(), getPermanentItemScene(), getAnyElementsAttributesProvider())) {

        bool ok = true;

        // TODO : perhaps we can accelerate the process when do this :
        //        - check if item draw enough lines/triangles/points/box to check first the bounding box. If true test the bounding box.
        /*if(getItem()->hasBoundingBox()) {
            Eigen::Vector3d bboxMin, bboxMax;
            getItem()->getBoundingBox(bboxMin, bboxMax);

            Eigen::Vector3d center((bboxMin[0]+bboxMax[0])/2.0, (bboxMin[1]+bboxMax[1])/2.0, (bboxMin[2]+bboxMax[2])/2.0);

            ok = checkBox(center[0], center[1], center[2],
                          bboxMax[0]-bboxMin[0],
                          bboxMax[1]-bboxMin[1],
                          bboxMax[2]-bboxMin[2],
                          Eigen::Vector3f(1, 0, 0),
                          Eigen::Vector3f(0, 1, 0),
                          false);
        }*/

        // TODO : Or we can accelerate the picking of item by using the opengl color picking techiques....

        if(ok) {

            AMKgl::ItemPickingPainter p(*this);

            getItem()->draw(*getGraphicsView(), p);

            selected = p.isSelected();
        }
    }

    emit finished();

    return selected;
}

void ObjectsOfItemPicker::dummySelectionOperation(Item *item,
                                                  ObjectsOfItemPicker::ItemInfo *info,
                                                  PermanentItemScene *scene,
                                                  AnyElementAttributesProvider *provider)
{
    Q_UNUSED(item)
    Q_UNUSED(info)
    Q_UNUSED(scene)
    Q_UNUSED(provider)
}

bool ObjectsOfItemPicker::dummyCheckOperation(const Item *item,
                                              const ObjectsOfItemPicker::ItemInfo *info,
                                              const PermanentItemScene *scene,
                                              const AnyElementAttributesProvider *provider)
{
    Q_UNUSED(item)
    Q_UNUSED(info)
    Q_UNUSED(scene)
    Q_UNUSED(provider)

    return true;
}
