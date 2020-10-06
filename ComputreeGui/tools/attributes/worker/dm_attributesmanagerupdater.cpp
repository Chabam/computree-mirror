#include "dm_attributesmanagerupdater.h"

#include "tools/attributes/dm_attributesbuildingcollectiont.h"
#include "tools/attributes/worker/dm_attributescolort.h"
#include "tools/attributes/worker/dm_attributesnormalt.h"
#include "tools/attributes/worker/dm_attributesscalart.h"

void DM_AttributesManagerUpdater::Update(DM_AttributesManager& manager)
{
    typedef DM_AttributesScalarT<CT_AbstractPointsAttributes>    PointAttributesScalar;
    typedef DM_AttributesColorT<CT_AbstractPointsAttributes>     PointAttributesColor;
    typedef DM_AttributesNormalT<CT_AbstractPointsAttributes>    PointAttributesNormal;

    typedef DM_AttributesScalarT<CT_AbstractFaceAttributes>      FaceAttributesScalar;
    typedef DM_AttributesColorT<CT_AbstractFaceAttributes>       FaceAttributesColor;
    typedef DM_AttributesNormalT<CT_AbstractFaceAttributes>      FaceAttributesNormal;

    typedef DM_AttributesScalarT<CT_AbstractEdgeAttributes>      EdgeAttributesScalar;
    typedef DM_AttributesColorT<CT_AbstractEdgeAttributes>       EdgeAttributesColor;
    typedef DM_AttributesNormalT<CT_AbstractEdgeAttributes>      EdgeAttributesNormal;

    manager.clearInvalid();

    const QList<CT_VirtualAbstractStep*> steps = GUI_MANAGER->getStepManager()->getStepRootList();

    for(CT_VirtualAbstractStep* step : steps)
    {
        UpdateWithStep< CT_AbstractPointsAttributes, PointAttributesScalar, PointAttributesColor, PointAttributesNormal >(manager, step);
        UpdateWithStep< CT_AbstractFaceAttributes, FaceAttributesScalar, FaceAttributesColor, FaceAttributesNormal >(manager, step);
        UpdateWithStep< CT_AbstractEdgeAttributes, EdgeAttributesScalar, EdgeAttributesColor, EdgeAttributesNormal >(manager, step);
    }
}

template<typename IAttributesType, typename AttributesScalarType, typename AttributesColorType, typename AttributesNormalType>
void DM_AttributesManagerUpdater::UpdateWithStep(DM_AttributesManager& manager, CT_VirtualAbstractStep* step)
{
    DM_AttributesBuildingCollectionT<IAttributesType> builderPoints;

    builderPoints.visitFrom(step, [&manager](const CT_OutAbstractModel* model, IAttributesType* pa) -> bool
    {
        CT_AbstractAttributesScalar* pas = dynamic_cast<CT_AbstractAttributesScalar*>(pa);

        if(pas != nullptr)
        {
            // local
            AttributesScalarType* localDPAS = static_cast<AttributesScalarType*>(manager.getAttributesFromInterface(pa, true));

            if(localDPAS == nullptr)
            {
                localDPAS = new AttributesScalarType(true);
                localDPAS->setTypeAttributes(pa, pas);
                localDPAS->setModel(model);

                manager.addAttributes(localDPAS);

                // global
                AttributesScalarType* globalDPAS = static_cast<AttributesScalarType*>(manager.getAttributesFromModel(model, false));

                if(globalDPAS == nullptr)
                {
                    globalDPAS = new AttributesScalarType(false);
                    globalDPAS->setTypeAttributes(pa, pas);
                    globalDPAS->setModel(model);

                    manager.addAttributes(globalDPAS);
                }
            }
        }
        else
        {
            CT_AttributesColor<IAttributesType>* pac = dynamic_cast<CT_AttributesColor<IAttributesType>*>(pa);

            if(pac != nullptr)
            {
                AttributesColorType* localDPAC = static_cast<AttributesColorType*>(manager.getAttributesFromInterface(pa, true));

                if(localDPAC == nullptr)
                {
                    localDPAC = new AttributesColorType(true);
                    localDPAC->setTypeAttributes(pa, pac);
                    localDPAC->setModel(model);

                    manager.addAttributes(localDPAC);

                    AttributesColorType* globalDPAC = static_cast<AttributesColorType*>(manager.getAttributesFromModel(model, false));

                    if(globalDPAC == nullptr)
                    {
                        globalDPAC = new AttributesColorType(false);
                        globalDPAC->setTypeAttributes(pa, pac);
                        globalDPAC->setModel(model);

                        manager.addAttributes(globalDPAC);
                    }
                }
            }
            else
            {
                CT_AttributesNormal<IAttributesType>* pan = dynamic_cast<CT_AttributesNormal<IAttributesType>*>(pa);

                if(pan != nullptr)
                {
                    AttributesNormalType* localDPAN = static_cast<AttributesNormalType*>(manager.getAttributesFromInterface(pa, true));

                    if(localDPAN == nullptr)
                    {
                        localDPAN = new AttributesNormalType(true);
                        localDPAN->setTypeAttributes(pa, pan);
                        localDPAN->setModel(model);

                        manager.addAttributes(localDPAN);

                        AttributesNormalType* globalDPAN = static_cast<AttributesNormalType*>(manager.getAttributesFromModel(model, false));

                        if(globalDPAN == nullptr)
                        {
                            globalDPAN = new AttributesNormalType(false);
                            globalDPAN->setTypeAttributes(pa, pan);
                            globalDPAN->setModel(model);

                            manager.addAttributes(globalDPAN);
                        }
                    }
                }
            }
        }

        return true;
    });
}

#if defined(_WIN32) && defined(_MSC_VER) // Microsoft Visual Studio Compiler
#pragma warning( disable : 4506 )
#elif (defined(__linux__) || defined(_WIN32)) && defined(__GNUC__) // GNU Compiler (gcc,g++) for Linux, Unix, and MinGW (Windows)
#elif defined(__APPLE__) // Clang Compiler (Apple)
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#endif
