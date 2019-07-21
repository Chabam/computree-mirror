#include "ct_stdpointsattributescontainer.h"

CT_StdPointsAttributesContainer::CT_StdPointsAttributesContainer() : SuperClass()
{
}

void CT_StdPointsAttributesContainer::insertPointsAttributesAt(const int &key, const CT_AbstractPointsAttributes *att)
{
    internalInsertAttributes(key, att);
}
