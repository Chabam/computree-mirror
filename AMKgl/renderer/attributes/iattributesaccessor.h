#ifndef IATTRIBUTESACCESSOR_H
#define IATTRIBUTESACCESSOR_H

#include "tools/anyelementattributesprovider.h"

class PermanentScene;

class IAttributesAccessor {
public:
    virtual ~IAttributesAccessor() {}

    /**
     * @brief Return the provider that get access to attributes cloud (colors, infos, normals) of objects like points, ellipses, etc...
     */
    virtual AnyElementAttributesProvider* getAnyElementsAttributesProvider() const = 0;

    /**
     * @brief Returns the provider of attributes (colors, infos, normals) of objects of specified type
     * @param type : type of element that you want
     */
    ObjectAttributesProvider* getObjectsAttributeProvider(Scene::ObjectType type) const {
        if(getAnyElementsAttributesProvider() == nullptr)
            return nullptr;

        return getAnyElementsAttributesProvider()->getObjectsAttributeProvider(type);
    }

    /**
     * @brief Returns the permanent scene used
     */
    virtual PermanentScene* getPermanentScene() const = 0;
};

#endif // IATTRIBUTESACCESSOR_H
