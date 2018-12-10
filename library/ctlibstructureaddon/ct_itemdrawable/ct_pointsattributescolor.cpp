#include "ct_pointsattributescolor.h"
#include "ct_colorcloud/ct_colorcloudstdvector.h"

CT_StandardPointsAttributesColorDrawManager CT_PointsAttributesColor::PAC_DRAW_MANAGER;

CT_PointsAttributesColor::CT_PointsAttributesColor() : CT_AbstractPointsAttributes(), CT_AttributesColor()
{
    setBaseDrawManager(&PAC_DRAW_MANAGER);
}

CT_PointsAttributesColor::CT_PointsAttributesColor(CT_PCIR pcir) : CT_AbstractPointsAttributes(pcir), CT_AttributesColor(new CT_ColorCloudStdVector(pcir->size()))
{
    setBaseDrawManager(&PAC_DRAW_MANAGER);
}

CT_PointsAttributesColor::CT_PointsAttributesColor(CT_PCIR pcir,
                                                   CT_AbstractColorCloud* cc) : CT_AbstractPointsAttributes(pcir), CT_AttributesColor(cc)
{
    setBaseDrawManager(&PAC_DRAW_MANAGER);
}
