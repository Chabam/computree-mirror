#include "ct_abstractobjectsvisitor.h"

CT_AbstractObjectsVisitor::CT_AbstractObjectsVisitor()
{
    m_follower = NULL;
}

void CT_AbstractObjectsVisitor::setFollower(const ObjectsVisitorFollowerInterface* follower)
{
    m_follower = (ObjectsVisitorFollowerInterface*)follower;
}

ObjectsVisitorFollowerInterface* CT_AbstractObjectsVisitor::getFollower() const
{
    return m_follower;
}
