#include "objectattributesprovider.h"

#include "renderer/ichunk.h"

ObjectAttributesProvider::ObjectAttributesProvider()
{
    m_accessor = nullptr;
    m_mutex = new QMutex(QMutex::Recursive);
}

ObjectAttributesProvider::~ObjectAttributesProvider()
{
    delete m_mutex;
}

void ObjectAttributesProvider::setObjectsRendererAttributesAccessor(const IObjectsRendererAttributesAccessor *accessor)
{
    m_accessor = (IObjectsRendererAttributesAccessor*)accessor;
}
