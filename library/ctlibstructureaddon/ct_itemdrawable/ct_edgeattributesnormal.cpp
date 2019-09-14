#include "ct_edgeattributesnormal.h"
#include "ct_normalcloud/ct_normalcloudstdvector.h"

CT_TYPE_IMPL_INIT_MACRO(CT_EdgeAttributesNormal)

CT_EdgeAttributesNormal::CT_EdgeAttributesNormal() : SuperClass(),
    CT_AttributesNormal()
{
}

CT_EdgeAttributesNormal::CT_EdgeAttributesNormal(CT_ECIR pcir) : SuperClass(pcir),
    CT_AttributesNormal(new CT_NormalCloudStdVector(pcir->size()))
{
}

CT_EdgeAttributesNormal::CT_EdgeAttributesNormal(CT_ECIR pcir,
                                                 CT_AbstractNormalCloud *nc,
                                                 bool autoDeleteNormalCloud) : SuperClass(pcir),
    CT_AttributesNormal(nc, autoDeleteNormalCloud)
{
}
