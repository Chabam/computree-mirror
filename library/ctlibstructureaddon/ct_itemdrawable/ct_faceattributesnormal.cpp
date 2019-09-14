#include "ct_faceattributesnormal.h"
#include "ct_normalcloud/ct_normalcloudstdvector.h"

CT_TYPE_IMPL_INIT_MACRO(CT_FaceAttributesNormal)

CT_FaceAttributesNormal::CT_FaceAttributesNormal() : SuperClass(),
    CT_AttributesNormal()
{
}

CT_FaceAttributesNormal::CT_FaceAttributesNormal(CT_FCIR pcir) : SuperClass(pcir),
    CT_AttributesNormal(new CT_NormalCloudStdVector(pcir->size()))
{
}

CT_FaceAttributesNormal::CT_FaceAttributesNormal(CT_FCIR pcir,
                                                 CT_AbstractNormalCloud *nc,
                                                 bool autoDeleteNormalCloud) : SuperClass(pcir),
    CT_AttributesNormal(nc, autoDeleteNormalCloud)
{
}
