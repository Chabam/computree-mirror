#ifndef DM_ATTRIBUTESMANAGERUPDATER_H
#define DM_ATTRIBUTESMANAGERUPDATER_H

#include "tools/attributes/worker/dm_attributesmanager.h"
#include "ct_step/abstract/ct_virtualabstractstep.h"

class DM_AttributesManagerUpdater
{
public:
    static void Update(DM_AttributesManager& manager);

private:
    template<typename IAttributesType, typename AttributesScalarType, typename AttributesColorType, typename AttributesNormalType>
    static void UpdateWithStep(DM_AttributesManager& manager, CT_VirtualAbstractStep* step);
};

#endif // DM_ATTRIBUTESMANAGERUPDATER_H
