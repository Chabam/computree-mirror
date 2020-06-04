#ifndef DM_ATTRIBUTESCOLORT_H
#define DM_ATTRIBUTESCOLORT_H

#include "tools/attributes/worker/abstract/dm_abstractattributescolor.h"

#include "dm_guimanager.h"

#include "view/DocumentView/gdocumentviewforgraphics.h"

#include "ct_attributes/ct_attributescolor.h"

#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"
#include "ct_colorcloud/ct_colorcloudstdvector.h"
#include "ct_cloudindex/ct_cloudindexlessmemoryt.h"

template<typename Type>
class DM_AttributesColorT : public DM_AbstractAttributesColor
{
public:
    DM_AttributesColorT(bool local);

    /**
     * @brief Check if the doc has the necessary cloud (color cloud) or set it if not
     */
    void checkAndSetNecessaryCloudToDoc() {}

    /**
     * @brief accept only Type (Point, Face, etc...) and colors
     */
    bool setTypeAttributes(const Type *ta, const CT_AttributesColor<Type> *ac);

    /**
     * @brief getter of I[Type]Attributes
     */
    CT_AttributesColor<Type>* colorAttributes() const;
    Type* abstractTypeAttributes() const;

protected:

    /**
     * @brief Apply the color to the [Type] of the document. Use [Type] cloud index from I[Type]Attributes.
     * @param doc : document to use
     */
    bool process(GDocumentViewForGraphics* doc) {  Q_UNUSED(doc) return false; }

    void finalProcess(GDocumentViewForGraphics* doc, AMKgl::GlobalColorCloud* ptrArray)
    {
        if(ptrArray == nullptr)
            return;

        AMKgl::GlobalColorCloud& refArray = *ptrArray;

        const size_t size = mustApplyToLocalIndex() ? m_ac->numberOfSetLocalValues() : m_ac->numberOfSetValues();

        if(size == 0)
            return;

        size_t i = 0;
        auto visitor = [this, &refArray, &i, &size](const size_t& globalIndex, const CT_Color& value) -> bool
        {
            refArray[globalIndex] = value;

            setProgress(int((i*100)/size));
            ++i;

            return !isCanceled();
        };

        if(mustApplyToLocalIndex())
            m_ac->visitLocalValues(visitor);
        else
            m_ac->visitValues(visitor);

        doc->dirtyColorsOfPoints();
    }

    void attributesDeleted();

private:
    CT_AttributesColor<Type>* m_ac;
};

// specialisation for points
template<>
bool DM_AttributesColorT<CT_AbstractPointsAttributes>::process(GDocumentViewForGraphics *doc);

// specialisation for faces
template<>
bool DM_AttributesColorT<CT_AbstractFaceAttributes>::process(GDocumentViewForGraphics *doc);

// specialisation for edges
template<>
bool DM_AttributesColorT<CT_AbstractEdgeAttributes>::process(GDocumentViewForGraphics *doc);

#include "tools/attributes/worker/dm_attributescolort.hpp"

#endif // DM_ATTRIBUTESCOLORT_H
