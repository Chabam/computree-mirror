#include "ct_pointsattributesnormal.h"
#include "ct_normalcloud/ct_normalcloudstdvector.h"

CT_StandardPointsAttributesNormalDrawManager CT_PointsAttributesNormal::PAN_DRAW_MANAGER;

CT_TYPE_IMPL_INIT_MACRO(CT_PointsAttributesNormal)

CT_PointsAttributesNormal::CT_PointsAttributesNormal() : SuperClass1(), SuperClass2()
{
    setBaseDrawManager(&PAN_DRAW_MANAGER);
}

CT_PointsAttributesNormal::CT_PointsAttributesNormal(CT_PCIR pcir) : SuperClass1(pcir),
    SuperClass2(new CT_NormalCloudStdVector(pcir->size()))
{
    setBaseDrawManager(&PAN_DRAW_MANAGER);
}

CT_PointsAttributesNormal::CT_PointsAttributesNormal(CT_PCIR pcir,
                                                     CT_AbstractNormalCloud* nc, bool autoDeleteNormalCloud) : SuperClass1(pcir), SuperClass2(nc, autoDeleteNormalCloud)
{
    setBaseDrawManager(&PAN_DRAW_MANAGER);
}
