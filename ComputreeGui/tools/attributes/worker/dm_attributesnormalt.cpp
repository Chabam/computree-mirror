#include "dm_attributesnormalt.h"

#include "ct_global/ct_context.h"
#include "ct_iterator/ct_pointiterator.h"
#include "ct_iterator/ct_faceiterator.h"
#include "ct_iterator/ct_edgeiterator.h"

#include "scene/permanentitemscenebymodel.h"
#include "tools/pointcloudattributesprovider.h"
#include "tools/facecloudattributesprovider.h"
#include "tools/edgecloudattributesprovider.h"

template<>
bool DM_AttributesNormalT<CT_AbstractPointsAttributes>::process(GDocumentViewForGraphics *doc)
{
    if(m_an == nullptr)
        return false;

    PermanentItemScene* scene = doc->getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr);

    if(scene == nullptr)
        return false;

    finalProcess(doc, scene->getPointCloudAttributesProvider()->createOrGetNormalCloud());
    return true;
}

template<>
bool DM_AttributesNormalT<CT_AbstractFaceAttributes>::process(GDocumentViewForGraphics *doc)
{
    if(m_an == nullptr)
        return false;

    PermanentItemScene* scene = doc->getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr);

    if(scene == nullptr)
        return false;

    finalProcess(doc, scene->getFaceCloudAttributesProvider()->createOrGetNormalCloud());
    return true;
}

template<>
bool DM_AttributesNormalT<CT_AbstractEdgeAttributes>::process(GDocumentViewForGraphics *doc)
{
    if(m_an == nullptr)
        return false;

    PermanentItemScene* scene = doc->getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr);

    if(scene == nullptr)
        return false;

    finalProcess(doc, scene->getEdgeCloudAttributesProvider()->createOrGetNormalCloud());
    return true;
}

template class DM_AttributesNormalT<CT_AbstractPointsAttributes>;
template class DM_AttributesNormalT<CT_AbstractFaceAttributes>;
template class DM_AttributesNormalT<CT_AbstractEdgeAttributes>;
