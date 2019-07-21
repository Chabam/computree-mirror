#include "ct_pointsattributescolor.h"
#include "ct_colorcloud/ct_colorcloudstdvector.h"

CT_StandardPointsAttributesColorDrawManager CT_PointsAttributesColor::PAC_DRAW_MANAGER;

CT_PointsAttributesColor::CT_PointsAttributesColor() : SuperClass1(), SuperClass2()
{
    setBaseDrawManager(&PAC_DRAW_MANAGER);
}

CT_PointsAttributesColor::CT_PointsAttributesColor(CT_PCIR pcir) : SuperClass1(pcir), SuperClass2(new CT_ColorCloudStdVector(pcir->size()))
{
    setBaseDrawManager(&PAC_DRAW_MANAGER);
}

CT_PointsAttributesColor::CT_PointsAttributesColor(CT_PCIR pcir,
                                                   CT_AbstractColorCloud* cc) : SuperClass1(pcir), SuperClass2(cc)
{
    setBaseDrawManager(&PAC_DRAW_MANAGER);
}
