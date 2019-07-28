#include "dm_attributescolort.h"

#include "scene/permanentitemscenebymodel.h"
#include "tools/pointcloudattributesprovider.h"
#include "tools/facecloudattributesprovider.h"
#include "tools/edgecloudattributesprovider.h"

template<>
bool DM_AttributesColorT<CT_AbstractPointsAttributes>::process(GDocumentViewForGraphics *doc)
{
    if(m_ac != nullptr)
    {
        const CT_AbstractCloudIndex *index = abstractTypeAttributes()->abstractCloudIndex();

        AMKgl::GlobalColorCloud* colorArray = doc->getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getPointCloudAttributesProvider()->createOrGetColorCloud();

        if(colorArray != nullptr)
        {
            size_t size = index->size();
            size_t globalIndexOfPoint;

            CT_AbstractColorCloud *toApplyColorCloud = m_ac->colorCloud();

            if(toApplyColorCloud != nullptr) {

                for(size_t i=0; i<size && !isCanceled(); ++i)
                {
                    index->indexAt(i, globalIndexOfPoint);
                    const CT_Color &color_pa = toApplyColorCloud->constColorAt(i);

                    // set the color of the Type at this document
                    (*colorArray)[globalIndexOfPoint] = color_pa;

                    setProgress(int((i*100)/size));
                }

                doc->dirtyColorsOfPoints();
            }

            return true;
        }
    }

    return false;
}

template<>
bool DM_AttributesColorT<CT_AbstractFaceAttributes>::process(GDocumentViewForGraphics *doc)
{
    if(m_ac != nullptr)
    {
        const CT_AbstractCloudIndex *index = abstractTypeAttributes()->abstractCloudIndex();

        AMKgl::GlobalColorCloud* colorArray = doc->getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getFaceCloudAttributesProvider()->createOrGetColorCloud();

        if(colorArray != nullptr)
        {
            size_t size = index->size();
            size_t globalIndexOfFace;

            CT_AbstractColorCloud *toApplyColorCloud = m_ac->colorCloud();

            if(toApplyColorCloud != nullptr) {

                for(size_t i=0; i<size && !isCanceled(); ++i)
                {
                    index->indexAt(i, globalIndexOfFace);
                    const CT_Color &color_pa = toApplyColorCloud->constColorAt(i);

                    // set the color of the Type at this document
                    (*colorArray)[globalIndexOfFace] = color_pa;

                    setProgress(int((i*100)/size));
                }
            }

            doc->dirtyColorsOfPoints();

            return true;
        }
    }

    return false;
}

template<>
bool DM_AttributesColorT<CT_AbstractEdgeAttributes>::process(GDocumentViewForGraphics *doc)
{
    if(m_ac != nullptr)
    {
        const CT_AbstractCloudIndex *index = abstractTypeAttributes()->abstractCloudIndex();

        AMKgl::GlobalColorCloud* colorArray = doc->getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getEdgeCloudAttributesProvider()->createOrGetColorCloud();

        if(colorArray != nullptr)
        {
            size_t size = index->size();
            size_t globalIndexOfEdge;

            CT_AbstractColorCloud *toApplyColorCloud = m_ac->colorCloud();

            if(toApplyColorCloud != nullptr) {

                for(size_t i=0; i<size && !isCanceled(); ++i)
                {
                    index->indexAt(i, globalIndexOfEdge);

                    const CT_Color &color_pa = toApplyColorCloud->constColorAt(i);

                    // set the color of the Type at this document
                    (*colorArray)[globalIndexOfEdge] = color_pa;


                    setProgress(int((i*100)/size));
                }
            }

            doc->dirtyColorsOfPoints();

            return true;
        }
    }

    return false;
}

template class DM_AttributesColorT<CT_AbstractPointsAttributes>;
template class DM_AttributesColorT<CT_AbstractFaceAttributes>;
template class DM_AttributesColorT<CT_AbstractEdgeAttributes>;
