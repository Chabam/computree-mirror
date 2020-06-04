#include "dm_attributesscalart.h"

template<>
void DM_AttributesScalarT<CT_AbstractPointsAttributes>::staticApply(ConcurrentMapInfo *info)
{
    auto visitor = [info](const size_t& globalIndex, double vv) -> bool
    {
        // convert the value to be between 0 and 1
        const double key = (vv-info->m_manualMin)/info->m_range;

        // get the intermediate color
        const QColor color = info->m_interpolator->intermediateColor(key);

        // set the color of the point at this document
        CT_Color &colorC = (*info->m_cc)[globalIndex];
        colorC = color;

        return true;
    };

    if(info->mApplyLocal)
        info->m_as->visitLocalValuesAsDouble(visitor);
    else
        info->m_as->visitValuesAsDouble(visitor);
}

template<>
void DM_AttributesScalarT<CT_AbstractFaceAttributes>::staticApply(ConcurrentMapInfo *info)
{
    auto visitor = [info](const size_t& globalIndex, double vv) -> bool
    {
        // convert the value to be between 0 and 1
        const double key = (vv-info->m_manualMin)/info->m_range;

        // get the intermediate color
        const QColor color = info->m_interpolator->intermediateColor(key);

        const CT_Face& face = info->m_fAccess->constFaceAt(globalIndex);

        // set the color of the face at this document
        CT_Color& colorA = (*info->m_cc)[face.iPointAt(0)];
        colorA = color;

        (*info->m_cc)[face.iPointAt(1)] = colorA;
        (*info->m_cc)[face.iPointAt(2)] = colorA;

        return true;
    };

    if(info->mApplyLocal)
        info->m_as->visitLocalValuesAsDouble(visitor);
    else
        info->m_as->visitValuesAsDouble(visitor);
}

template<>
void DM_AttributesScalarT<CT_AbstractEdgeAttributes>::staticApply(ConcurrentMapInfo *info)
{
    auto visitor = [info](const size_t& globalIndex, double vv) -> bool
    {
        // convert the value to be between 0 and 1
        const double key = (vv-info->m_manualMin)/info->m_range;

        // get the intermediate color
        const QColor color = info->m_interpolator->intermediateColor(key);

        const CT_Edge &edge = info->m_eAccess->constEdgeAt(globalIndex);

        // set the color of the edge at this document
        CT_Color &colorA = (*info->m_cc)[edge.iPointAt(0)];
        colorA = color;

        (*info->m_cc)[edge.iPointAt(1)] = colorA;

        return true;
    };

    if(info->mApplyLocal)
        info->m_as->visitLocalValuesAsDouble(visitor);
    else
        info->m_as->visitValuesAsDouble(visitor);
}

template class DM_AttributesScalarT<CT_AbstractPointsAttributes>;
template class DM_AttributesScalarT<CT_AbstractFaceAttributes>;
template class DM_AttributesScalarT<CT_AbstractEdgeAttributes>;
