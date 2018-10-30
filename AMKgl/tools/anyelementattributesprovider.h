#ifndef ANYELEMENTATTRIBUTESPROVIDER_H
#define ANYELEMENTATTRIBUTESPROVIDER_H

#include "scene/permanentsceneelementtype.h"

#include <array>

class PointCloudAttributesProvider;
class FaceCloudAttributesProvider;
class EdgeCloudAttributesProvider;
class ObjectAttributesProvider;

/**
 * @brief Provider for attributes of any elements (points/faces/edges/ellipses/quads/etc...)
 */
class AnyElementAttributesProvider
{
public:
    AnyElementAttributesProvider();
    ~AnyElementAttributesProvider();

    /**
     * @brief Returns the provider of attributes of an element
     * @param type : type of element that you want
     */
    ObjectAttributesProvider* getObjectsAttributeProvider(Scene::ObjectType type) const;

private:
    std::array<ObjectAttributesProvider*, Scene::NumberOfElements>    m_objectsAttributesProvider;
};

#endif // ANYELEMENTATTRIBUTESPROVIDER_H
