#include "ct_pointsattributesnormal.h"
#include "ct_normalcloud/ct_normalcloudstdvector.h"

CT_StandardPointsAttributesNormalDrawManager CT_PointsAttributesNormal::PAN_DRAW_MANAGER;

CT_PointsAttributesNormal::CT_PointsAttributesNormal() : CT_AbstractPointsAttributes(), CT_AttributesNormal()
{
    setBaseDrawManager(&PAN_DRAW_MANAGER);
}

CT_PointsAttributesNormal::CT_PointsAttributesNormal(CT_PCIR pcir) : CT_AbstractPointsAttributes(pcir),
    CT_AttributesNormal(new CT_NormalCloudStdVector(pcir->size()))
{
    setBaseDrawManager(&PAN_DRAW_MANAGER);
}

CT_PointsAttributesNormal::CT_PointsAttributesNormal(CT_PCIR pcir,
                                                     CT_AbstractNormalCloud* nc) : CT_AbstractPointsAttributes(pcir), CT_AttributesNormal(nc)
{
    setBaseDrawManager(&PAN_DRAW_MANAGER);
}
