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
    if(m_an == NULL)
        return false;

    CT_PointIterator it(abstractTypeAttributes()->abstractCloudIndex());

    size_t size = it.size();

    AMKgl::GlobalNormalCloud* normalArray = doc->getPermanentSceneToRender()->getPermanentItemSceneForModel(NULL)->getPointCloudAttributesProvider()->createOrGetNormalCloud();

    if(normalArray != NULL)
    {
        CT_AbstractNormalCloud* toApplyNormalCloud = m_an->getNormalCloud();

        if(toApplyNormalCloud != NULL) {

            size_t i = 0;
            while(it.hasNext() && !isCanceled())
            {
                size_t indexP = it.next().cIndex();

                const CT_Normal &nxnynz_pa = toApplyNormalCloud->constNormalAt(i);

                // set the normal of the point at this document
                (*normalArray)[indexP] = nxnynz_pa;

                setProgress(int((i*100)/size));
                ++i;
            }

            doc->dirtyNormalsOfPoints();
        }

        return true;
    }

    return false;
}

template<>
bool DM_AttributesNormalT<CT_AbstractFaceAttributes>::process(GDocumentViewForGraphics *doc)
{
    if(m_an == NULL)
        return false;

    CT_FaceIterator it(abstractTypeAttributes()->abstractCloudIndex());

    size_t size = it.size();

    AMKgl::GlobalNormalCloud* normalArray = doc->getPermanentSceneToRender()->getPermanentItemSceneForModel(NULL)->getFaceCloudAttributesProvider()->createOrGetNormalCloud();

    if(normalArray != NULL)
    {
        CT_AbstractNormalCloud* toApplyNormalCloud = m_an->getNormalCloud();

        if(toApplyNormalCloud != NULL) {

            size_t i = 0;
            while(it.hasNext() && !isCanceled())
            {
                size_t indexF = it.next().cIndex();

                const CT_Normal &nxnynz_pa = toApplyNormalCloud->constNormalAt(i);

                // set the normal of the face at this document
                (*normalArray)[indexF] = nxnynz_pa;

                setProgress(int((i*100)/size));
                ++i;
            }

            doc->dirtyNormalsOfPoints();
        }

        return true;
    }

    return false;
}

template<>
bool DM_AttributesNormalT<CT_AbstractEdgeAttributes>::process(GDocumentViewForGraphics *doc)
{
    if(m_an == NULL)
        return false;

    CT_EdgeIterator it(abstractTypeAttributes()->abstractCloudIndex());

    size_t size = it.size();

    AMKgl::GlobalNormalCloud* normalArray = doc->getPermanentSceneToRender()->getPermanentItemSceneForModel(NULL)->getEdgeCloudAttributesProvider()->createOrGetNormalCloud();

    if(normalArray != NULL)
    {
        CT_AbstractNormalCloud* toApplyNormalCloud = m_an->getNormalCloud();

        if(toApplyNormalCloud != NULL) {

            size_t i = 0;
            while(it.hasNext() && !isCanceled())
            {
                size_t indexE = it.next().cIndex();

                const CT_Normal &nxnynz_pa = toApplyNormalCloud->constNormalAt(i);

                // set the normal of the face at this document
                (*normalArray)[indexE] = nxnynz_pa;

                setProgress(int((i*100)/size));
                ++i;
            }

            doc->dirtyNormalsOfPoints();
        }

        return true;
    }

    return false;
}

template class DM_AttributesNormalT<CT_AbstractPointsAttributes>;
template class DM_AttributesNormalT<CT_AbstractFaceAttributes>;
template class DM_AttributesNormalT<CT_AbstractEdgeAttributes>;
