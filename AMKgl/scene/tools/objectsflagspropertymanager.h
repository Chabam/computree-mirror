#ifndef OBJECTSFLAGSPROPERTYMANAGER_H
#define OBJECTSFLAGSPROPERTYMANAGER_H

#include "renderer/tools/objectsflagproperty.h"
#include "picker/tools/objectsflagstool.h"
#include "scene/permanentsceneelementtype.h"

class PermanentScene;

/**
 * @brief Class used to manage flags property
 */
class ObjectsFlagsPropertyManager
{
public:
    ObjectsFlagsPropertyManager(PermanentScene& scene);

    /**
     * @brief Set property for the specified flag to all type of objects
     */
    void setProperty(const ObjectsFlagsTool::Flag& flag,
                     const ObjectsFlagProperty& property);

    /**
     * @brief Set property for the specified flag for the specified object
     */
    void setProperty(const ObjectsFlagsTool::Flag& flag,
                     const Scene::ObjectType& typeOfObject,
                     const ObjectsFlagProperty& property);

    /**
     * @brief Returns the flag property currently used for the specified flag and specified object
     */
    ObjectsFlagProperty getProperty(const ObjectsFlagsTool::Flag& flag,
                                    const Scene::ObjectType& typeOfObject) const;

    /**
     * @brief Returns the flag property currently used for the specified flag and all objects
     * @param ok: false if one object type don't use the same property, true if all objects type use the same property.
     * @warning if "ok" is false the property returned can be undetermined
     */
    ObjectsFlagProperty getProperty(const ObjectsFlagsTool::Flag& flag,
                                    bool& ok) const;

private:
    PermanentScene& m_scene;
};

#endif // OBJECTSFLAGSPROPERTYMANAGER_H
