#include "ct_edgeattributesnormal.h"
#include "ct_normalcloud/ct_normalcloudstdvector.h"

CT_EdgeAttributesNormal::CT_EdgeAttributesNormal() : SuperClass(),
    CT_AttributesNormal()
{
}

CT_EdgeAttributesNormal::CT_EdgeAttributesNormal(CT_ECIR pcir) : SuperClass(pcir),
    CT_AttributesNormal(new CT_NormalCloudStdVector(pcir->size()))
{
}

CT_EdgeAttributesNormal::CT_EdgeAttributesNormal(CT_ECIR pcir,
                                                 CT_AbstractNormalCloud *nc) : SuperClass(pcir),
    CT_AttributesNormal(nc)
{
}
