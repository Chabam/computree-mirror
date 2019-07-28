#include "bufferobjectmanager.h"

#include <cstring>

BufferObjectManager::BufferObjectManager()
{
}

BufferObjectManager::~BufferObjectManager()
{
    deleteAllBO();
}

bool BufferObjectManager::processBOThatMustBeUpdated()
{
    bool atLeastOneBOUpdated = false;

    BufferObjectCollectionIterator it(m_bufferObjects);

    while(it.hasNext()) {
        BOAndProperty* boPro = it.next();

        if(boPro->m_toBeUpdated) {
            atLeastOneBOUpdated = true;
            boPro->m_toBeUpdated = false;

            QOpenGLBuffer& bo = *boPro->m_bo;

            if(bo.isCreated()) {
                if(bo.bind()) {
                    qint8* fv = (qint8*)bo.map(QOpenGLBuffer::WriteOnly);
                    qint8* fvToMap = (qint8*)boPro->getFirstValue();

                    if((fv != nullptr) && (fvToMap != nullptr))
                        std::memcpy(fv, fvToMap, boPro->getArraySize() * boPro->getSizeOfOneValueInByte());

                    bo.unmap();
                    bo.release();
                }
            }
        }
    }

    return atLeastOneBOUpdated;
}

bool BufferObjectManager::createBO(void* objectThatCreateIt,
                                   const size_t &uniqueIndex,
                                   const QOpenGLBuffer::Type &boType,
                                   funcGetFirstValue func,
                                   const size_t& sizeOfValueInByte,
                                   const size_t& arraySize)
{
    Q_ASSERT(func != nullptr);

    if(func(objectThatCreateIt) == nullptr)
        return false;

    BOAndProperty& boPro = createNewOrGetSharedBufferObjectFor(objectThatCreateIt,
                                                               uniqueIndex,
                                                               boType,
                                                               func,
                                                               sizeOfValueInByte,
                                                               arraySize);

    QOpenGLBuffer& bo = *boPro.m_bo;

    if(boPro.getArraySize() == arraySize) {
        if(bo.isCreated())
            return true;
    } else {
        boPro.changeArraySize(arraySize);

        if(bo.isCreated())
            return updateBO(boType, boPro.getFirstValue(), arraySize);
    }

    bool ok = bo.create();

    if(ok)
        ok = bo.bind();

    if(ok) {
        bo.setUsagePattern(QOpenGLBuffer::StaticDraw);
        bo.allocate(boPro.getFirstValue(), (int)(arraySize * sizeOfValueInByte));
        bo.release();
    }

    return ok;
}

QOpenGLBuffer& BufferObjectManager::getBO(void* objectThatCreateIt,
                                          const size_t& uniqueIndex,
                                          const QOpenGLBuffer::Type& boType) const
{
    BufferObjectCollectionIterator it(m_bufferObjects);

    while(it.hasNext()) {
        BOAndProperty* boPro = it.next();

        if(boPro->m_bo->type() == boType) {
            if(boPro->containsOwner(objectThatCreateIt, uniqueIndex))
                return *boPro->m_bo;
        }
    }

    return const_cast<QOpenGLBuffer&>(m_dumbBuffer);
}

bool BufferObjectManager::updateBO(const QOpenGLBuffer::Type& boType,
                                   void* firstValue,
                                   const size_t& arraySize)
{
    bool ok = false;

    if(firstValue != nullptr) {

        int boIndex = searchSharedBufferObject(boType, firstValue, arraySize);

        if(boIndex < 0)
            return false;

        BOAndProperty* boPro = m_bufferObjects.at(boIndex);
        boPro->m_toBeUpdated = true;
    }

    return ok;
}

void BufferObjectManager::deleteAllBOFor(void* objectThatCreateIt,
                                         const size_t& uniqueIndex,
                                         const QOpenGLBuffer::Type& boType)
{
    BufferObjectCollectionMutableIterator it(m_bufferObjects);

    while(it.hasNext()) {
        BOAndProperty* boPro = it.next();

        if(boPro->m_bo->type() == boType) {
            if(boPro->removeOwner(objectThatCreateIt, uniqueIndex)) {
                if(boPro->m_owners.isEmpty()) {
                    it.remove();
                    delete boPro;
                }
            }
        }
    }
}

void BufferObjectManager::deleteAllBOFor(void *objectThatCreateIt,
                                         const QOpenGLBuffer::Type &boType)
{
    BufferObjectCollectionMutableIterator it(m_bufferObjects);

    while(it.hasNext()) {
        BOAndProperty* boPro = it.next();

        if(boPro->m_bo->type() == boType) {
            if(boPro->removeOwner(objectThatCreateIt)) {
                if(boPro->m_owners.isEmpty()) {
                    it.remove();
                    delete boPro;
                }
            }
        }
    }
}

void BufferObjectManager::deleteAllBOFor(void *objectThatCreateIt)
{
    BufferObjectCollectionMutableIterator it(m_bufferObjects);

    while(it.hasNext()) {
        BOAndProperty* boPro = it.next();

        if(boPro->removeOwner(objectThatCreateIt)) {
            if(boPro->m_owners.isEmpty()) {
                it.remove();
                delete boPro;
            }
        }
    }
}

void BufferObjectManager::deleteAllBO()
{
    qDeleteAll(m_bufferObjects.begin(), m_bufferObjects.end());
    m_bufferObjects.clear();
}

int BufferObjectManager::searchSharedBufferObject(const QOpenGLBuffer::Type &boType,
                                                  void* firstValue)
{
    int index = 0;

    BufferObjectCollectionIterator it(m_bufferObjects);

    while(it.hasNext()) {
        const BOAndProperty* boPro = it.next();
        const void* otherFirstValue = boPro->getFirstValue();

        if((otherFirstValue != nullptr)
                && (boPro->getFirstValue() == firstValue)
                && (boPro->m_bo->type() == boType)) {
            return index;
        }

        ++index;
    }

    return -1;
}

int BufferObjectManager::searchSharedBufferObject(const QOpenGLBuffer::Type& boType,
                                                  void* firstValue,
                                                  const size_t& arraySize)
{
    int index = 0;

    BufferObjectCollectionIterator it(m_bufferObjects);

    while(it.hasNext()) {
        const BOAndProperty* boPro = it.next();

        if((boPro->getFirstValue() == firstValue)
                && (boPro->getArraySize() == arraySize)
                && (boPro->m_bo->type() == boType)) {
            return index;
        }

        ++index;
    }

    return -1;
}

BufferObjectManager::BOAndProperty& BufferObjectManager::createNewOrGetSharedBufferObjectFor(void* objectThatCreateIt,
                                                                                             const size_t& uniqueIndex,
                                                                                             const QOpenGLBuffer::Type& boType,
                                                                                             funcGetFirstValue func,
                                                                                             const size_t& sizeOfValueInByte,
                                                                                             const size_t& arraySize)
{
    BOOwnerInfo newOwner(objectThatCreateIt, func, uniqueIndex, arraySize);

    int sharedIndex = searchSharedBufferObject(boType, newOwner.getFirstValue());

    if(sharedIndex >= 0) {
        BOAndProperty* boPro = m_bufferObjects.at(sharedIndex);
        boPro->addOwner(newOwner);

        return *boPro;
    }

    BOAndProperty* boPro = new BOAndProperty(boType,
                                             arraySize,
                                             sizeOfValueInByte,
                                             newOwner);

    m_bufferObjects.append(boPro);

    return *boPro;
}

BufferObjectManager::BOOwnerInfo::BOOwnerInfo(void* objectThatCreateIt,
                                              funcGetFirstValue func,
                                              const size_t& uniqueIndex,
                                              const size_t& arraySize)
{
    Q_ASSERT(func != nullptr);

    m_objectThatCreateIt = objectThatCreateIt;
    m_getFirstValueFunc = func;
    m_uniqueIndex = uniqueIndex;
    m_arraySize = arraySize;
}

BufferObjectManager::BOOwnerInfo::BOOwnerInfo(void *objectThatCreateIt, const size_t &uniqueIndex)
{
    m_objectThatCreateIt = objectThatCreateIt;
    m_getFirstValueFunc = nullptr;
    m_uniqueIndex = uniqueIndex;
    m_arraySize = 0;
}

bool BufferObjectManager::BOOwnerInfo::operator ==(const BufferObjectManager::BOOwnerInfo &other) const
{
    return (other.m_objectThatCreateIt == m_objectThatCreateIt) && (other.m_uniqueIndex == m_uniqueIndex);
}

void* BufferObjectManager::BOOwnerInfo::getFirstValue() const
{
    return m_getFirstValueFunc(m_objectThatCreateIt);
}

BufferObjectManager::BOAndProperty::BOAndProperty(const QOpenGLBuffer::Type& boType,
                                                  const size_t& arraySize,
                                                  const size_t& sizeOfValueInByte,
                                                  const BOOwnerInfo& firstOwner)
{
    m_bo = new QOpenGLBuffer(boType);
    m_arraySize = arraySize;
    m_sizeOfValueInByte = sizeOfValueInByte;
    m_toBeUpdated = false;

    addOwner(firstOwner);
}

BufferObjectManager::BOAndProperty::~BOAndProperty()
{
    delete m_bo;
}

void BufferObjectManager::BOAndProperty::addOwner(const BufferObjectManager::BOOwnerInfo& owner)
{
    if(m_owners.contains(owner))
        return;

    m_owners.append(owner);
}

bool BufferObjectManager::BOAndProperty::removeOwner(void* objectThatCreateIt,
                                                     const size_t& uniqueIndex)
{
    BOOwnerInfo owner(objectThatCreateIt, uniqueIndex);

    size_t maxArraySize = 0;

    QMutableListIterator<BOOwnerInfo> it(m_owners);

    while(it.hasNext()) {
        const BOOwnerInfo& info = it.next();

        if(info == owner)
            it.remove();
        else
            maxArraySize = qMax(maxArraySize, info.m_arraySize);
    }

    changeArraySize(maxArraySize);

    return false;
}

bool BufferObjectManager::BOAndProperty::removeOwner(void* objectThatCreateIt)
{
    bool ok = false;
    size_t maxArraySize = 0;

    QMutableListIterator<BOOwnerInfo> it(m_owners);

    while(it.hasNext()) {
        const BOOwnerInfo& info = it.next();

        if(info.m_objectThatCreateIt == objectThatCreateIt) {
            it.remove();
            ok = true;
        } else {
            maxArraySize = qMax(maxArraySize, info.m_arraySize);
        }
    }

    changeArraySize(maxArraySize);

    return ok;
}

bool BufferObjectManager::BOAndProperty::containsOwner(void* objectThatCreateIt, const size_t& uniqueIndex) const
{
    return m_owners.contains(BOOwnerInfo(objectThatCreateIt, uniqueIndex));
}

void BufferObjectManager::BOAndProperty::changeArraySize(const size_t &newSize)
{
    m_arraySize = newSize;
}

size_t BufferObjectManager::BOAndProperty::getArraySize() const
{
    return m_arraySize;
}

size_t BufferObjectManager::BOAndProperty::getSizeOfOneValueInByte() const
{
    return m_sizeOfValueInByte;
}

void* BufferObjectManager::BOAndProperty::getFirstValue() const
{
    Q_ASSERT(!m_owners.isEmpty());

    return m_owners.first().getFirstValue();
}
