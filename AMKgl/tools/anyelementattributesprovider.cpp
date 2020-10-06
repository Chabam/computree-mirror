#include "anyelementattributesprovider.h"

#include "objectattributesprovider.h"

AnyElementAttributesProvider::AnyElementAttributesProvider()
{
    for(int i=0; i<static_cast<int>(m_objectsAttributesProvider.size()); ++i)
        m_objectsAttributesProvider[i] = new ObjectAttributesProvider();
}

AnyElementAttributesProvider::~AnyElementAttributesProvider()
{
    qDeleteAll(m_objectsAttributesProvider.begin(), m_objectsAttributesProvider.end());
}

ObjectAttributesProvider* AnyElementAttributesProvider::getObjectsAttributeProvider(Scene::ObjectType type) const
{
    return m_objectsAttributesProvider[(int)type];
}
