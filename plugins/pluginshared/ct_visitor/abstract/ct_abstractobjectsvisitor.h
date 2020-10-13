#ifndef CT_ABSTRACTOBJECTSVISITOR_H
#define CT_ABSTRACTOBJECTSVISITOR_H

#include "interfaces.h"
#include "pluginShared_global.h"

class PLUGINSHAREDSHARED_EXPORT CT_AbstractObjectsVisitor : public ObjectsVisitorInterface
{
public:
    CT_AbstractObjectsVisitor();

    /**
     * @brief Set the follower that allow ou to get some informations and the possibility
     *        to create a objects modifier
     */
    void setFollower(const ObjectsVisitorFollowerInterface* follower);

    /**
     * @brief Returns the follower
     */
    ObjectsVisitorFollowerInterface* getFollower() const;

    /**
     * @brief Returns the number of objects available
     */
    inline size_t getNumberOfObjects() const { return m_follower->getNumberOfObjects(); }

    /**
     * @brief Returns the number of vertex per object
     */
    inline size_t getNumberOfVertexPerObject() const { return m_follower->getNumberOfVertexPerObject(); }

    /**
     * @brief Returns the current object type
     */
    inline GraphicsObjectType currentObjectType() const { return m_follower->currentObjectType(); }

    /**
     * @brief Create an interface that allow the developper to query the status of all new objects
     */
    inline QSharedPointer<ObjectStatusQueryiorInterface> createObjectsStatusQueryior() { return m_follower->createObjectsStatusQueryior(); }

    /**
     * @brief Create an interface that allow the developper to modify all new objects
     */
    inline QSharedPointer<ObjectsModifierInterface> createObjectsModifier() { return m_follower->createObjectsModifier(); }

private:
    ObjectsVisitorFollowerInterface*    m_follower;
};

#endif // CT_ABSTRACTOBJECTSVISITOR_H
