#ifndef IOBJECTSRENDERERATTRIBUTESACCESSOR_H
#define IOBJECTSRENDERERATTRIBUTESACCESSOR_H

#include "iobjectstypeaccessor.h"
#include "renderer/generic/generictypedef.h"

class IChunk;

/**
 * @brief Interface that must inherit an object renderer to give an access to color cloud, normal cloud, info cloud, etc...
 * @todo TODO : remove this class because it was not necessary !
 */
class IObjectsRendererAttributesAccessor : public IObjectsTypeAccessor
{
public:
    typedef Basic::LocalColor                Color;
    typedef Basic::LocalInfo                 Info;

    typedef Basic::AbstractColorCloud   AbstractColorCloud;
    typedef Basic::AbstractNormalCloud  AbstractNormalCloud;
    typedef Basic::AbstractInfoCloud    AbstractInfoCloud;

    virtual ~IObjectsRendererAttributesAccessor() {}
};

#endif // IOBJECTSRENDERERATTRIBUTESACCESSOR_H
