#include "meshobjectssetinfovisitor.h"

#include "renderer/mesh/abstractchunkgenericcloudobject.h"
#include "scene/permanentitemscenebymodel.h"

MeshObjectsSetInfoVisitor::MeshObjectsSetInfoVisitor(Scene::ObjectType type)
{
    Q_ASSERT((type == Scene::TriangleGlobalCloud) || (type == Scene::LineGlobalCloud));   
    if ((type != Scene::TriangleGlobalCloud) && (type != Scene::LineGlobalCloud)) {qDebug() << "MeshObjectsSetInfoVisitor::MeshObjectsSetInfoVisitor" << ", " <<  "(type != Scene::TriangleGlobalCloud) && (type != Scene::LineGlobalCloud)"; return;}

    m_objectType = type;
    m_setFunction = nullptr;
    m_createInfoCloud = true;
}

void MeshObjectsSetInfoVisitor::setFunction(MeshObjectsSetInfoVisitor::functionForSet f)
{
    m_setFunction = f;
}

void MeshObjectsSetInfoVisitor::setCreateInfoItNotCreated(bool createIt)
{
    m_createInfoCloud = createIt;
}

void MeshObjectsSetInfoVisitor::compute(PermanentItemSceneByModel *scene)
{
    scene->visitObjectsOfTypes(QList<Scene::ObjectType>() << m_objectType, *this);
}

void MeshObjectsSetInfoVisitor::visitChunk(const IChunk* chunk)
{
    Q_ASSERT(m_setFunction != nullptr);
    if (m_setFunction == nullptr) {qDebug() << "MeshObjectsSetInfoVisitor::visitChunk" << ", " <<  "m_setFunction == nullptr"; return;}

    const AbstractChunkGenericCloudObject* objectChunk = dynamic_cast<const AbstractChunkGenericCloudObject*>(chunk);

    ElementInfo* foin = objectChunk->getFirstObjectInfo();

    if(m_createInfoCloud && (foin == nullptr))
        foin = objectChunk->createOrGetFirstObjectInfo();

    if(foin != nullptr) {
        const size_t nObjects = objectChunk->countObjects();

        GLuint* foi = objectChunk->getFirstObjectIndex();

        for(size_t i=0; i<nObjects; ++i)
            m_setFunction(foin[foi[i]]);
    }
}
