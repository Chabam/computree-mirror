#include "dm_attributescolort.h"

#include "scene/permanentitemscenebymodel.h"
#include "tools/pointcloudattributesprovider.h"
#include "tools/facecloudattributesprovider.h"
#include "tools/edgecloudattributesprovider.h"

template<>
bool DM_AttributesColorT<CT_AbstractPointsAttributes>::process(GDocumentViewForGraphics* doc)
{
    if(m_ac == nullptr)
        return false;

    PermanentItemScene* scene = doc->getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr);

    if(scene == nullptr)
        return false;

    finalProcess(doc, scene->getPointCloudAttributesProvider()->createOrGetColorCloud());
    return true;
}

template<>
bool DM_AttributesColorT<CT_AbstractFaceAttributes>::process(GDocumentViewForGraphics* doc)
{
    if(m_ac == nullptr)
        return false;

    PermanentItemScene* scene = doc->getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr);

    if(scene == nullptr)
        return false;

    finalProcess(doc, scene->getFaceCloudAttributesProvider()->createOrGetColorCloud());
    return true;
}

template<>
bool DM_AttributesColorT<CT_AbstractEdgeAttributes>::process(GDocumentViewForGraphics* doc)
{
    if(m_ac == nullptr)
        return false;

    PermanentItemScene* scene = doc->getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr);

    if(scene == nullptr)
        return false;

    finalProcess(doc, scene->getEdgeCloudAttributesProvider()->createOrGetColorCloud());
    return true;
}

template class DM_AttributesColorT<CT_AbstractPointsAttributes>;
template class DM_AttributesColorT<CT_AbstractFaceAttributes>;
template class DM_AttributesColorT<CT_AbstractEdgeAttributes>;
