#ifndef DISPLAYLISTMANAGER_H
#define DISPLAYLISTMANAGER_H

#include <QtOpenGL>

/**
 * @brief Use this class to manage display list
 */
class DisplayListManager
{
public:
    DisplayListManager();
    ~DisplayListManager();

    /**
     * @brief Generate a new display list for and return it or just return it if it was already created. You can create
     *        as many dislay list as you want, you must just have a different unique index for each list.
     * @param uniqueIndex : a unique index that you must use to retreive the generated list
     */
    GLuint createOrGetDisplayList(const size_t& uniqueIndex);

    /**
     * @brief This will execute the display list with specified unique index with "glCallList". If this list doesn't
     *        exist it will automatically be created with call to method "id = createOrGetDisplayList()" and
     *        glNewList(id, GL_COMPILE_AND_EXECUTE). After that if you call this method il will use "glCallList". If
     *        you want to destroy the list you must call "destroyGL".
     *
     *        example :
     *
     *        if(!executeOrCreateAndCompileAndExecuteDisplayList(myUniqueIndex)) {
     *              drawMyObject();
     *              endDisplayList(myUniqueIndex);
     *        }
     * @warning Don't forget to call "endDisplayList" if this method returns false !
     * @param uniqueIndex : the unique index used in method "createOrGetDisplayList" or if you don't call this method use a unique index.
     * @return false if you must call your draw method, true if not
     */
    bool executeOrCreateAndCompileAndExecuteDisplayList(const size_t& uniqueIndex);

    /**
     * @brief Call this method after you draw your object if "executeOrCreateAndCompileAndExecuteDisplayList" return false. Otherwise
     *        you don't have to call this method.
     */
    void endDisplayList(const size_t& uniqueIndex);

    /**
     * @brief Delete a display list
     */
    void deleteDisplayList(const size_t& uniqueIndex);

    /**
     * @brief Delete all display list
     */
    void deleteAllDisplayList();

private:
    typedef QHash<size_t, qint64>           DisplayListCollection;
    typedef QHashIterator<size_t, qint64>   DisplayListCollectionIterator;

    DisplayListCollection                   m_displayListsHandle;
};

#endif // DISPLAYLISTMANAGER_H
