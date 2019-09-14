#include "ct_faceattributescolor.h"
#include "ct_colorcloud/ct_colorcloudstdvector.h"

CT_DEFAULT_IA_INIT(CT_FaceAttributesColor)

CT_FaceAttributesColor::CT_FaceAttributesColor() : SuperClass(),
    CT_AttributesColor()
{
}

CT_FaceAttributesColor::CT_FaceAttributesColor(CT_FCIR pcir) : SuperClass(pcir),
    CT_AttributesColor(new CT_ColorCloudStdVector(pcir->size()))
{
}

CT_FaceAttributesColor::CT_FaceAttributesColor(CT_FCIR pcir,
                                               CT_AbstractColorCloud *cc,
                                               bool autoDeleteColorCloud) : SuperClass(pcir),
    CT_AttributesColor(cc, autoDeleteColorCloud)
{
}
