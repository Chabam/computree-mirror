#ifndef DM_ATTRIBUTESNORMALT_H
#define DM_ATTRIBUTESNORMALT_H

#include "tools/attributes/worker/abstract/dm_abstractattributesnormal.h"
#include "view/DocumentView/gdocumentviewforgraphics.h"
#include "dm_guimanager.h"

#include "ct_attributes/ct_attributesnormal.h"
#include "ct_normalcloud/registered/ct_standardnormalcloudregistered.h"
#include "ct_normalcloud/abstract/ct_abstractnormalcloud.h"

template<typename Type>
class DM_AttributesNormalT : public DM_AbstractAttributesNormal
{
public:
    DM_AttributesNormalT(bool local);

    /**
     * @brief Check if the doc has the necessary cloud (normal cloud) or set it if not
     */
    void checkAndSetNecessaryCloudToDoc() {}

    /**
     * @brief accept only Type (Point, Face, etc...) and normals
     */
    bool setTypeAttributes(const Type *ta, const CT_AttributesNormal<Type> *an);

    /**
     * @brief getter of I[Type]Attributes
     */
    CT_AttributesNormal<Type>* normalAttributes() const;
    Type* abstractTypeAttributes() const;

protected:

    /**
     * @brief Apply the normal to the [Type] of the document. Use [Type] cloud index from I[Type]Attributes.
     * @param doc : document to use
     */
    bool process(GDocumentViewForGraphics* doc) {  Q_UNUSED(doc) return false; }

    void finalProcess(GDocumentViewForGraphics* doc, AMKgl::GlobalNormalCloud* ptrArray)
    {
        if(ptrArray == nullptr)
            return;

        AMKgl::GlobalNormalCloud& refArray = *ptrArray;

        const size_t size = mustApplyToLocalIndex() ? m_an->numberOfSetLocalValues() : m_an->numberOfSetValues();

        if(size == 0)
            return;

        size_t i = 0;
        auto visitor = [this, &refArray, &i, &size](const size_t& globalIndex, const CT_Normal& value) -> bool
        {
            refArray[globalIndex] = value;

            setProgress(int((i*100)/size));
            ++i;

            return !isCanceled();
        };

        if(mustApplyToLocalIndex())
            m_an->visitLocalValues(visitor);
        else
            m_an->visitValues(visitor);

        doc->dirtyNormalsOfPoints();
    }

    void attributesDeleted();

private:
    CT_AttributesNormal<Type> *m_an;
};

// specialisation for points
template<>
bool DM_AttributesNormalT<CT_AbstractPointsAttributes>::process(GDocumentViewForGraphics *doc);

// specialisation for faces
template<>
bool DM_AttributesNormalT<CT_AbstractFaceAttributes>::process(GDocumentViewForGraphics *doc);

// specialisation for edges
template<>
bool DM_AttributesNormalT<CT_AbstractEdgeAttributes>::process(GDocumentViewForGraphics *doc);

#include "tools/attributes/worker/dm_attributesnormalt.hpp"

#endif // DM_ATTRIBUTESNORMALT_H
