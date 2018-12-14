#include "ct_edgeattributescolor.h"
#include "ct_colorcloud/ct_colorcloudstdvector.h"

CT_EdgeAttributesColor::CT_EdgeAttributesColor() : SuperClass(), CT_AttributesColor()
{
}

CT_EdgeAttributesColor::CT_EdgeAttributesColor(CT_ECIR pcir) : SuperClass(pcir),
    CT_AttributesColor(new CT_ColorCloudStdVector(pcir->size()))
{
}

CT_EdgeAttributesColor::CT_EdgeAttributesColor(CT_ECIR pcir,
                                               CT_AbstractColorCloud *cc) : SuperClass(pcir),
    CT_AttributesColor(cc)
{
}
