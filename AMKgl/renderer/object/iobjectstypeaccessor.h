#ifndef IOBJECTSTYPEACCESSOR_H
#define IOBJECTSTYPEACCESSOR_H

#include "scene/permanentsceneelementtype.h"

class IObjectsTypeAccessor
{
public:
    virtual ~IObjectsTypeAccessor() {}

    /**
     * @brief Return the type of object draw by this renderer
     */
    virtual Scene::ObjectType getTypeOfObjectDrawn() const = 0;
};
#endif // IOBJECTSTYPEACCESSOR_H
