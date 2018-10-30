#include "displaylistmanager.h"

DisplayListManager::DisplayListManager()
{
}

DisplayListManager::~DisplayListManager()
{
    deleteAllDisplayList();
}

GLuint DisplayListManager::createOrGetDisplayList(const size_t& uniqueIndex)
{
    qint64 dlid = m_displayListsHandle.value(uniqueIndex, -1);

    if(dlid < 0) {
        dlid = glGenLists(1);
        m_displayListsHandle.insert(uniqueIndex, GLuint(dlid));
    }

    return GLuint(dlid);
}

bool DisplayListManager::executeOrCreateAndCompileAndExecuteDisplayList(const size_t& uniqueIndex)
{
    qint64 dlid = m_displayListsHandle.value(uniqueIndex, -1);

    if(dlid == 0) {
        return false;
    } else if(dlid > 0){
        glCallList(GLuint(dlid));
        return true;
    }

    dlid = createOrGetDisplayList(uniqueIndex);

    if(dlid != 0)
        glNewList(GLuint(dlid), GL_COMPILE_AND_EXECUTE);

    return false;
}

void DisplayListManager::endDisplayList(const size_t& uniqueIndex)
{
    Q_UNUSED(uniqueIndex);

    glEndList();
}

void DisplayListManager::deleteDisplayList(const size_t &uniqueIndex)
{
    qint64 dlid = m_displayListsHandle.value(uniqueIndex, -1);

    if(dlid == -1)
        return;

    if(dlid > 0)
        glDeleteLists(dlid, 1);

    m_displayListsHandle.remove(uniqueIndex);
}

void DisplayListManager::deleteAllDisplayList()
{
    DisplayListCollectionIterator it(m_displayListsHandle);

    while(it.hasNext()) {
        qint64 dlid = it.next().value();

        if(dlid > 0)
            glDeleteLists(dlid, 1);
    }

    m_displayListsHandle.clear();
}
