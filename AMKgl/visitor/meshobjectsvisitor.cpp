#include "meshobjectsvisitor.h"

#include "renderer/mesh/abstractchunkgenericcloudobject.h"
#include "scene/permanentitemscenebymodel.h"

MeshObjectsVisitor::MeshObjectsVisitor(Scene::ObjectType type)
{
    Q_ASSERT((type == Scene::TriangleGlobalCloud) || (type == Scene::LineGlobalCloud));
    if ((type != Scene::TriangleGlobalCloud) && (type != Scene::LineGlobalCloud)) {qDebug() << "MeshObjectsVisitor::MeshObjectsVisitor" << ", " <<  "(type != Scene::TriangleGlobalCloud) && (type != Scene::LineGlobalCloud)"; return;}

    m_objectType = type;
    m_visitFunction = nullptr;
}

void MeshObjectsVisitor::setFunction(MeshObjectsVisitor::functionForVisit f)
{
    m_visitFunction = f;
}

void MeshObjectsVisitor::compute(PermanentItemSceneByModel *scene)
{
    scene->visitObjectsOfTypes(QList<Scene::ObjectType>() << m_objectType, *this);
}

void MeshObjectsVisitor::visitChunk(const IChunk* chunk)
{
    Q_ASSERT(m_visitFunction != nullptr);
    if (m_visitFunction == nullptr) {qDebug() << "MeshObjectsVisitor::visitChunk" << ", " <<  "m_visitFunction == nullptr"; return;}

    const AbstractChunkGenericCloudObject* objectChunk = dynamic_cast<const AbstractChunkGenericCloudObject*>(chunk);

    if(objectChunk != nullptr)
        m_visitFunction((AbstractChunkGenericCloudObject*)objectChunk);
}
