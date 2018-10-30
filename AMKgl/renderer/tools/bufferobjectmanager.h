#ifndef BUFFEROBJECTMANAGER_H
#define BUFFEROBJECTMANAGER_H

#include <QtOpenGL>

#include <functional>

/**
 * @brief This class must be a singleton per opengl context. It will manage all buffer object created
 *        to keep the minimum memory consumption in the graphics card.
 *
 *        To do this it will compare buffer object that was created and if it was same as a previously
 *        created it will be shared. Two buffer object is considered same if the pointer of the first
 *        value and the size of the array is same.
 */
class BufferObjectManager
{
public:
    typedef std::function<void* (void* objectThatCreateIt)> funcGetFirstValue;

    BufferObjectManager();
    ~BufferObjectManager();

    /**
     * @brief Call this method in the preDraw() method to update all BO that must to be.
     * @return True if at least one bo has been updated, false otherwise
     */
    bool processBOThatMustBeUpdated();

    /**
     * @brief Create a buffer object that will contains all values. It will be shared if it is considered the same BO as another created previously.
     * @param objectThatCreateIt : pass "this" pointer
     * @param uniqueIndex : a unique index. If you want to create multiple BO you can pass multiple index that must be unique.
     * @param boType : the buffer object type (vertex, color, etc...)
     * @param funcGetFirstValue : pointer to a function that get the first value in the array to store in the buffer object
     * @param sizeOfValueInByte : size of one value in byte ==> sizeof(XXX)
     * @param arraySize : array size (number of values)
     * @return true if buffer object was created successfully. False if an error append
     *         or if the buffer object was already created.
     */
    bool createBO(void* objectThatCreateIt,
                  const size_t& uniqueIndex,
                  const QOpenGLBuffer::Type& boType,
                  funcGetFirstValue func,
                  const size_t& sizeOfValueInByte,
                  const size_t& arraySize);

    /**
     * @brief Returns the buffer object created for the associated object, unique index and bo type
     * @param objectThatCreateIt
     * @param uniqueIndex
     * @return
     */
    QOpenGLBuffer& getBO(void* objectThatCreateIt,
                         const size_t& uniqueIndex,
                         const QOpenGLBuffer::Type& boType) const;

    /**
     * @brief Update the buffer object that contains all values.
     * @param boType : the buffer object type (vertex, color, etc...)
     * @param firstValue : pointer to the first value in the array (it will be used to find the associated BO)
     * @param arraySize : array size (number of vertex)
     * @return true if buffer object was updated successfully. False if an error append or if "firstValue" is NULL.
     */
    bool updateBO(const QOpenGLBuffer::Type& boType,
                  void* firstValue,
                  const size_t& arraySize);

    /**
     * @brief Call when you will be destroyed (in your destructor) or when you don't want to reuse the BO
     * @param objectThatCreateIt : pass "this" pointer
     * @param uniqueIndex : the unique index used when you create it.
     * @param boType : the buffer object type (vertex, color, etc...) used when you create it.
     */
    void deleteAllBOFor(void* objectThatCreateIt,
                        const size_t& uniqueIndex,
                        const QOpenGLBuffer::Type& boType);

    /**
     * @brief Call when you will be destroyed (in your destructor) or when you don't want to reuse all created BO
     * @param objectThatCreateIt : pass "this" pointer
     * @param boType : the buffer object type (vertex, color, etc...) used when you create it.
     */
    void deleteAllBOFor(void* objectThatCreateIt,
                        const QOpenGLBuffer::Type& boType);

    /**
     * @brief Call when you will be destroyed (in your destructor) or when you don't want to reuse all created BO
     * @param objectThatCreateIt : pass "this" pointer
     */
    void deleteAllBOFor(void* objectThatCreateIt);

    /**
     * @brief Call when you want to destroy all BO created
     */
    void deleteAllBO();

    /**
     * @brief Create a buffer object that will contains all values. It will be shared if it is considered the same BO as another created previously.
     * @param objectThatCreateIt : pass "this" pointer
     * @param uniqueIndex : a unique index. If you want to create multiple BO you can pass multiple index that must be unique.
     * @param boType : the buffer object type (vertex, color, etc...)
     * @param funcGetFirstValue : pointer to a function that get the first value in the array to store in the buffer object
     * @param arraySize : array size (number of vertex)
     * @return true if buffer object was created successfully. False if an error append
     *         or if the buffer object was already created.
     */
    template<typename ValueType>
    bool createBO(void* objectThatCreateIt,
                  const size_t& uniqueIndex,
                  const QOpenGLBuffer::Type& boType,
                  funcGetFirstValue func,
                  const size_t& arraySize)
    {
        return createBO(objectThatCreateIt,
                        uniqueIndex,
                        boType,
                        func,
                        sizeof(ValueType),
                        arraySize);
    }

    /**
     * @brief Just a shorcut, see "createBO" method for more information
     */
    template<typename ValueType>
    bool createVertexBO(void* objectThatCreateIt,
                        const size_t& uniqueIndex,
                        funcGetFirstValue func,
                        const size_t& arraySize)
    {
        return createBO<ValueType>(objectThatCreateIt,
                                   uniqueIndex,
                                   QOpenGLBuffer::VertexBuffer,
                                   func,
                                   arraySize);
    }

    /**
     * @brief Just a shorcut, see "createBO" method for more information
     */
    template<typename ValueType>
    bool createIndexBO(void* objectThatCreateIt,
                       const size_t& uniqueIndex,
                       funcGetFirstValue func,
                       const size_t& arraySize)
    {
        return createBO<ValueType>(objectThatCreateIt,
                                   uniqueIndex,
                                   QOpenGLBuffer::IndexBuffer,
                                   func,
                                   arraySize);
    }

    /**
     * @brief Just a shorcut, see "createBO" method for more information
     */
    template<typename ValueType>
    bool createInfosBO(void* objectThatCreateIt,
                       const size_t& uniqueIndex,
                       funcGetFirstValue func,
                       const size_t& arraySize)
    {
        return createVertexBO<ValueType>(objectThatCreateIt,
                                         uniqueIndex,
                                         func,
                                         arraySize);
    }

    /**
     * @brief Just a shorcut, see "createBO" method for more information
     */
    template<typename ValueType>
    bool createColorsBO(void* objectThatCreateIt,
                        const size_t& uniqueIndex,
                        funcGetFirstValue func,
                        const size_t& arraySize)
    {
        return createVertexBO<ValueType>(objectThatCreateIt,
                                         uniqueIndex,
                                         func,
                                         arraySize);
    }

    /**
     * @brief Just a shorcut, see "createBO" method for more information
     */
    template<typename ValueType>
    bool createNormalsBO(void* objectThatCreateIt,
                         const size_t& uniqueIndex,
                         funcGetFirstValue func,
                         const size_t& arraySize)
    {
        return createVertexBO<ValueType>(objectThatCreateIt,
                                         uniqueIndex,
                                         func,
                                         arraySize);
    }

    /**
     * @brief Just a shorcut, see "getBO" method for more information
     */
    QOpenGLBuffer& getVertexBO(void* objectThatCreateIt,
                               const size_t& uniqueIndex) const
    {
        return getBO(objectThatCreateIt, uniqueIndex, QOpenGLBuffer::VertexBuffer);
    }

    /**
     * @brief Just a shorcut, see "getBO" method for more information
     */
    QOpenGLBuffer& getIndexBO(void* objectThatCreateIt,
                              const size_t& uniqueIndex) const
    {
        return getBO(objectThatCreateIt, uniqueIndex, QOpenGLBuffer::IndexBuffer);
    }

    /**
     * @brief Just a shorcut, see "getBO" method for more information
     */
    QOpenGLBuffer& getInfosBO(void* objectThatCreateIt,
                              const size_t& uniqueIndex) const
    {
        return getVertexBO(objectThatCreateIt, uniqueIndex);
    }

    /**
     * @brief Just a shorcut, see "getBO" method for more information
     */
    QOpenGLBuffer& getColorsBO(void* objectThatCreateIt,
                               const size_t& uniqueIndex) const
    {
        return getVertexBO(objectThatCreateIt, uniqueIndex);
    }

    /**
     * @brief Just a shorcut, see "getBO" method for more information
     */
    QOpenGLBuffer& getNormalsBO(void* objectThatCreateIt,
                              const size_t& uniqueIndex) const
    {
        return getVertexBO(objectThatCreateIt, uniqueIndex);
    }

    /**
     * @brief Just a shorcut, see "updateBO" method for more information
     */
    template<typename ValueType>
    bool updateVertexBO(ValueType* firstValue,
                        const size_t& arraySize)
    {
        return updateBO(QOpenGLBuffer::VertexBuffer,
                        (void*)firstValue,
                        arraySize);
    }

    /**
     * @brief Just a shorcut, see "updateBO" method for more information
     */
    template<typename ValueType>
    bool updateIndexBO(ValueType* firstValue,
                       const size_t& arraySize)
    {
        return updateBO(QOpenGLBuffer::IndexBuffer,
                        (void*)firstValue,
                        arraySize);
    }

    /**
     * @brief Just a shorcut, see "updateBO" method for more information
     */
    template<typename ValueType>
    bool updateInfosBO(ValueType* firstValue,
                       const size_t& arraySize)
    {
        return updateVertexBO(firstValue,
                              arraySize);
    }

    /**
     * @brief Just a shorcut, see "updateBO" method for more information
     */
    template<typename ValueType>
    bool updateColorsBO(ValueType* firstValue,
                        const size_t& arraySize)
    {
        return updateVertexBO(firstValue,
                              arraySize);
    }

    /**
     * @brief Just a shorcut, see "updateBO" method for more information
     */
    template<typename ValueType>
    bool updateNormalsBO(ValueType* firstValue,
                         const size_t& arraySize)
    {
        return updateVertexBO(firstValue,
                              arraySize);
    }

private:

    struct BOOwnerInfo {

        BOOwnerInfo(void* objectThatCreateIt,
                    funcGetFirstValue func,
                    const size_t& uniqueIndex,
                    const size_t& arraySize);

        // only for comparison !
        BOOwnerInfo(void* objectThatCreateIt,
                    const size_t& uniqueIndex);

        bool operator ==(const BOOwnerInfo& other) const;

        void* getFirstValue() const;

        void*               m_objectThatCreateIt;
        funcGetFirstValue   m_getFirstValueFunc;
        size_t              m_uniqueIndex;
        size_t              m_arraySize;
    };

    struct BOAndProperty {
        BOAndProperty(const QOpenGLBuffer::Type& boType,
                      const size_t& arraySize,
                      const size_t& sizeOfValueInByte,
                      const BOOwnerInfo& firstOwner);
        ~BOAndProperty();

        void addOwner(const BOOwnerInfo& owner);
        bool removeOwner(void* objectThatCreateIt, const size_t& uniqueIndex);
        bool removeOwner(void* objectThatCreateIt);
        bool containsOwner(void* objectThatCreateIt, const size_t& uniqueIndex) const;
        void changeArraySize(const size_t& newSize);
        size_t getArraySize() const;
        size_t getSizeOfOneValueInByte() const;
        void* getFirstValue() const;

        QOpenGLBuffer*          m_bo;
        QList<BOOwnerInfo>      m_owners;
        bool                    m_toBeUpdated;

    private:
        size_t                  m_sizeOfValueInByte;
        size_t                  m_arraySize;
    };

    typedef QList<BOAndProperty*>                   BufferObjectCollection;
    typedef QListIterator<BOAndProperty*>           BufferObjectCollectionIterator;
    typedef QMutableListIterator<BOAndProperty*>    BufferObjectCollectionMutableIterator;

    /**
     * @brief Collection of buffers created
     */
    BufferObjectCollection m_bufferObjects;

    /**
     * @brief Buffer that was returned when nothing was founded
     */
    QOpenGLBuffer           m_dumbBuffer;

    /**
     * @brief Search a buffer object that can be shared (the size of the array can be different)
     * @param boType : the buffer object type (vertex, color, etc...)
     * @param firstValue : pointer to the first value in the array
     * @return -1 if not shared buffer was founded, otherwise return the index of the BO in the collection that can be used.
     */
    int searchSharedBufferObject(const QOpenGLBuffer::Type& boType,
                                 void* firstValue);

    /**
     * @brief Search a buffer object that can be shared (the size of the array must be equal)
     * @param boType : the buffer object type (vertex, color, etc...)
     * @param firstValue : pointer to the first value in the array
     * @param arraySize : array size
     * @return -1 if not shared buffer was founded, otherwise return the index of the BO in the collection that can be used.
     */
    int searchSharedBufferObject(const QOpenGLBuffer::Type& boType,
                                 void* firstValue,
                                 const size_t& arraySize);

    /**
     * @brief Find a buffer object that can be shared and return it or if it was not founded create a new and return it
     * @param objectThatCreateIt : pass "this" pointer
     * @param uniqueIndex : a unique index. If you want to create multiple BO you can pass multiple index that must be unique.
     * @param boType : the buffer object type (vertex, color, etc...)
     * @param funcGetFirstValue : pointer to a function that get the first value in the array to store in the buffer object
     * @param sizeOfValueInByte : size of one value in byte ==> sizeof(XXX)
     * @param arraySize : array size
     * @return Returns the BO and property that must be used.
     */
    BOAndProperty& createNewOrGetSharedBufferObjectFor(void* objectThatCreateIt,
                                                       const size_t& uniqueIndex,
                                                       const QOpenGLBuffer::Type& boType,
                                                       funcGetFirstValue func,
                                                       const size_t& sizeOfValueInByte,
                                                       const size_t& arraySize);


    /**
     * @brief Find a buffer object that can be shared and return it or if it was not founded create a new and return it
     * @param objectThatCreateIt : pass "this" pointer
     * @param uniqueIndex : a unique index. If you want to create multiple BO you can pass multiple index that must be unique.
     * @param boType : the buffer object type (vertex, color, etc...)
     * @param funcGetFirstValue : pointer to a function that get the first value in the array to store in the buffer object
     * @param arraySize : array size
     * @return Returns the BO and property that must be used.
     */
    template<typename ValueType>
    BOAndProperty& createNewOrGetSharedBufferObjectFor(void* objectThatCreateIt,
                                                       const size_t& uniqueIndex,
                                                       const QOpenGLBuffer::Type& boType,
                                                       funcGetFirstValue func,
                                                       const size_t& arraySize)
    {
        return createNewOrGetSharedBufferObjectFor(objectThatCreateIt,
                                                   uniqueIndex,
                                                   boType,
                                                   func,
                                                   sizeof(ValueType),
                                                   arraySize);
    }
};

#endif // BUFFEROBJECTMANAGER_H
