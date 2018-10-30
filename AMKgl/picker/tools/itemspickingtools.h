#ifndef ITEMSPICKINGTOOLS_H
#define ITEMSPICKINGTOOLS_H

#include "scene/tools/permanentiteminformation.h"
#include "tools/anyelementattributesprovider.h"
#include "tools/objectattributesprovider.h"
#include "tools/pointcloudattributesprovider.h"
#include "objectsflagstool.h"
#include "renderer/ichunk.h"
#include "scene/tools//dispatchinformation.h"
#include "includeglobalcloud.h"
#include "defineitem.h"

using namespace AMKgl;

/**
 * @brief Use static method of this class to check if an item
 *        is selected or not, to select/unselect an item.
 */
class ItemsPickingTools
{
public:
    typedef std::function<void (ObjectAttributesProvider* provider,
                                DispatchInformation *objInfo,
                                ObjectAttributesProvider::AbstractInfoCloud *cloud,
                                const size_t& objectIndex)> functionToApplyToInfo;

    /**
     * @brief Select the item
     */
    static inline void staticSelectItem(ObjectAttributesProvider* provider,
                                        DispatchInformation *objInfo,
                                        ObjectAttributesProvider::AbstractInfoCloud *cloud,
                                        const size_t& objectIndex) {
        Q_UNUSED(provider)
        Q_UNUSED(objInfo)

        ObjectAttributesProvider::AbstractInfoCloud::return_type v = (*cloud)[objectIndex];
        ObjectsFlagsTool::staticSelectObject(v);
    }

    /**
     * @brief Select the point of the item
     */
    static inline void staticSelectPointOfItem(PointCloudAttributesProvider* provider,
                                               ElementInfo* firstPointInfo,
                                               GLuint* firstIndexOfPoints,
                                               const size_t& nThPoint) {
        Q_UNUSED(provider)

        ObjectsFlagsTool::staticSelectObject(firstPointInfo[firstIndexOfPoints[nThPoint]]);
    }

    /**
     * @brief Unselect the item
     */
    static inline void staticUnselectItem(ObjectAttributesProvider* provider,
                                          DispatchInformation *objInfo,
                                          ObjectAttributesProvider::AbstractInfoCloud *cloud,
                                          const size_t& objectIndex) {
        Q_UNUSED(provider)
        Q_UNUSED(objInfo)

        ObjectAttributesProvider::AbstractInfoCloud::return_type v = (*cloud)[objectIndex];
        ObjectsFlagsTool::staticUnselectObject(v);
    }

    /**
     * @brief Select the point of the item
     */
    static inline void staticUnselectPointOfItem(PointCloudAttributesProvider* provider,
                                                 ElementInfo* firstPointInfo,
                                                 GLuint* firstIndexOfPoints,
                                                 const size_t& nThPoint) {
        Q_UNUSED(provider)

        ObjectsFlagsTool::staticUnselectObject(firstPointInfo[firstIndexOfPoints[nThPoint]]);
    }

    /**
     * @brief An item added objects like ellipses, lines, quads, etc... in permanent renderer. Each object has an
     *        attribute that was used to know if it was selected or not or if it was visible or not, etc....
     *        This method will apply the operation specified in parameter to each attribute of the item defined by
     *        "info" parameter. It will use the "provider" to get the attribute cloud and the "info" to get what attribute
     *        to pass to the function.
     *
     * @param item : item
     * @param info : item's information
     * @param provider : provider to get cloud to modify
     * @param f : function to apply (can be NULL)
     */
    static inline void staticApplyOperationToObjectOfAnItem(Item* item,
                                                            PermanentItemInformation* info,
                                                            AnyElementAttributesProvider* provider,
                                                            ItemsPickingTools::functionToApplyToInfo f) {

        Q_UNUSED(item)

        if(f != NULL) {
            PermanentItemInformation::ObjectCollectionIterator it(info->getObjectsCollection());

            while(it.hasNext()) {
                it.next();

                DispatchInformation *objInfo = it.value();

                const size_t count = objInfo->count();

                if(count != 0) {

                    IChunk* chunk = it.key();

                    ObjectAttributesProvider* oProvider = provider->getObjectsAttributeProvider(chunk->getTypeOfObjectDrawn());

                    ObjectAttributesProvider::AbstractInfoCloud* cloud = chunk->createOrGetObjectInfoCloud();

                    size_t index = objInfo->begin();

                    for(size_t k=0; k<count; ++k)
                        f(oProvider, objInfo, cloud, index++);
                }
            }
        }
    }

    /**
     * @brief Same as the method with one profider but apply the operation to each attribute
     *        in cloud of each provider of the list.
     *
     * @param item : item
     * @param info : item's information
     * @param providers : collection of provider (AnyElementAttributesProvider*) to use to get cloud to modify
     * @param f : function to apply
     */
    template<typename Collection>
    static inline void staticApplyOperationToObjectOfAnItem(Item* item,
                                                            PermanentItemInformation* info,
                                                            const Collection& providers,
                                                            ItemsPickingTools::functionToApplyToInfo f) {

        Q_UNUSED(item)

        typename Collection::const_iterator begin = providers.begin();
        typename Collection::const_iterator it = begin;
        typename Collection::const_iterator end = providers.end();

        if(f != NULL) {
            PermanentItemInformation::ObjectCollectionIterator itO(info->getObjectsCollection());

            while(itO.hasNext()) {
                itO.next();

                DispatchInformation *objInfo = itO.value();

                const size_t count = objInfo->count();

                if(count != 0) {

                    IChunk* chunk = itO.key();

                    it = begin;

                    while(it != end) {

                        ObjectAttributesProvider* oProvider = (*it)->getObjectsAttributeProvider(chunk->getTypeOfObjectDrawn());

                        Basic::AbstractInfoCloud* cloud = chunk->createOrGetObjectInfoCloud();

                        size_t index = objInfo->begin();

                        for(size_t k=0; k<count; ++k)
                            f(oProvider, objInfo, cloud, index++);

                        ++it;
                    }
                }
            }
        }
    }

    static inline void staticApplyOperationToPointsOfAnItem(Item* item,
                                                            PermanentItemInformation* info,
                                                            AnyElementAttributesProvider* provider,
                                                            ItemsPickingTools::functionToApplyToInfo f) {
        Q_UNUSED(item)

        if(f != NULL) {
            PermanentItemInformation::ObjectCollectionIterator it(info->getObjectsCollection());

            while(it.hasNext()) {
                it.next();

                DispatchInformation *objInfo = it.value();

                const size_t count = objInfo->count();

                if(count != 0) {

                    IChunk* chunk = it.key();

                    if(chunk->getTypeOfObjectDrawn() == Scene::PointGlobalCloud) {
                        ObjectAttributesProvider* oProvider = provider->getObjectsAttributeProvider(chunk->getTypeOfObjectDrawn());

                        ObjectAttributesProvider::AbstractInfoCloud* cloud = chunk->createOrGetObjectInfoCloud();

                        size_t index = objInfo->begin();

                        for(size_t k=0; k<count; ++k)
                            f(oProvider, objInfo, cloud, index++);
                    }
                }
            }
        }
    }
};

#endif // ITEMSPICKINGTOOLS_H
