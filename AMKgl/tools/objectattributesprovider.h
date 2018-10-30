#ifndef OBJECTATTRIBUTESPROVIDER_H
#define OBJECTATTRIBUTESPROVIDER_H

#include "renderer/object/iobjectsrendererattributesaccessor.h"

class IChunk;

/**
 * @brief Provider for attributes (normals, infos and colors) of objects (points, ellipses, cylinders, quads, etc...)
 * @todo TODO : remove it because this class was not necessary !
 */
class ObjectAttributesProvider
{
public:
    typedef IObjectsRendererAttributesAccessor::AbstractColorCloud          AbstractColorCloud;
    typedef IObjectsRendererAttributesAccessor::AbstractNormalCloud         AbstractNormalCloud;
    typedef IObjectsRendererAttributesAccessor::AbstractInfoCloud           AbstractInfoCloud;

    /**
     * @brief Construct an object attributes provider
     */
    ObjectAttributesProvider();
    ~ObjectAttributesProvider();

    /**
     * @brief Set the accessor of clouds
     */
    void setObjectsRendererAttributesAccessor(const IObjectsRendererAttributesAccessor* accessor);

private:
    IObjectsRendererAttributesAccessor* m_accessor;
    QMutex*                             m_mutex;
};

#endif // OBJECTATTRIBUTESPROVIDER_H
