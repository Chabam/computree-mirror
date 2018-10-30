#include "objectsflagspropertymanager.h"

#include "scene/permanentscene.h"

ObjectsFlagsPropertyManager::ObjectsFlagsPropertyManager(PermanentScene& scene) : m_scene(scene)
{
}

void ObjectsFlagsPropertyManager::setProperty(const ObjectsFlagsTool::Flag &flag,
                                              const ObjectsFlagProperty &property)
{
    for(int i=0; i<Scene::NumberOfElements; ++i)
        m_scene.getRendererForType(Scene::ObjectType(i))->getFlagsPropertyManager()->setProperty(flag, property);
}

void ObjectsFlagsPropertyManager::setProperty(const ObjectsFlagsTool::Flag &flag,
                                              const Scene::ObjectType &typeOfObject,
                                              const ObjectsFlagProperty &property)
{
    m_scene.getRendererForType(typeOfObject)->getFlagsPropertyManager()->setProperty(flag, property);
}

ObjectsFlagProperty ObjectsFlagsPropertyManager::getProperty(const ObjectsFlagsTool::Flag &flag,
                                                             const Scene::ObjectType &typeOfObject) const
{
    return m_scene.getRendererForType(typeOfObject)->getFlagsPropertyManager()->getProperty(flag);
}

ObjectsFlagProperty ObjectsFlagsPropertyManager::getProperty(const ObjectsFlagsTool::Flag &flag,
                                                             bool &ok) const
{
    ok = false;

    ObjectsFlagProperty firstProperty = getProperty(flag, Scene::ObjectType(0));

    for(int i=1; i<Scene::NumberOfElements; ++i) {

        ObjectsFlagProperty property = getProperty(flag, Scene::ObjectType(i));

        if(property != firstProperty)
            return firstProperty;
    }

    ok = true;
    return firstProperty;
}
