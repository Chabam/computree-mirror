#ifndef IEASYACCESS_H
#define IEASYACCESS_H

#include "iamkgllog.h"
#include "renderer/tools/bufferobjectmanager.h"

class ICoordinateSystemManager;
class IGlobalPointCloudManager;
class IGlobalEdgeCloudManager;
class IGlobalFaceCloudManager;

/**
 * @brief Inherit this class to get easy access to essential elements used by this engine
 */
class IEasyAccess
{
public:
    virtual ~IEasyAccess() {
        qDeleteAll(m_bufferObjectManagers.begin(), m_bufferObjectManagers.end());
    }

    template<typename ICloudManager>
    ICloudManager* getGlobalCloudManager() const {
        Q_ASSERT_X(false, "IEasyAccess::getGlobalCloudManager", "Developper forget to specialize this method with a special type of global cloud manager.");
        return nullptr;
    }

    /**
     * @brief Returns the global point cloud manager
     */
    virtual IGlobalPointCloudManager* getGlobalPointCloudManager() const = 0;

    /**
     * @brief Returns the global edge cloud manager
     */
    virtual IGlobalEdgeCloudManager* getGlobalEdgeCloudManager() const = 0;

    /**
     * @brief Returns the global face cloud manager
     */
    virtual IGlobalFaceCloudManager* getGlobalFaceCloudManager() const = 0;

    /**
     * @brief Returns the coordinate system manager
     */
    virtual ICoordinateSystemManager* getCoordinateSystemManager() const = 0;

    /**
     * @brief Returns the log system
     */
    virtual IAMKglLog* getLogger() const { return &const_cast<IAMKglLog&>(m_defaultLogger); }

    /**
     * @brief Return the buffer object manager
     */
    BufferObjectManager& createOrGetBufferObjectManager(const QOpenGLContext* context)
    {
        QOpenGLContext* ncContext = const_cast<QOpenGLContext*>(context);
        BufferObjectManager* manager = m_bufferObjectManagers.value(ncContext, nullptr);

        if(manager == nullptr) {
            manager = new BufferObjectManager();
            m_bufferObjectManagers.insert(ncContext, manager);
        }

        return *manager;
    }

    /**
     * @brief Call it when a context is about to be deleted
     */
    void contextAboutToBeDeleted(const QOpenGLContext* context)
    {
        QOpenGLContext* ncContext = const_cast<QOpenGLContext*>(context);
        BufferObjectManager* manager = m_bufferObjectManagers.value(ncContext, nullptr);

        if(manager != nullptr)
            m_bufferObjectManagers.remove(ncContext);

        delete manager;
    }

private:
    IAMKglLog                                       m_defaultLogger;
    QHash<QOpenGLContext*, BufferObjectManager*>    m_bufferObjectManagers;
};

// specialisation
template<>
IGlobalPointCloudManager* IEasyAccess::getGlobalCloudManager() const;

template<>
IGlobalEdgeCloudManager* IEasyAccess::getGlobalCloudManager() const;

template<>
IGlobalFaceCloudManager* IEasyAccess::getGlobalCloudManager() const;

#endif // IEASYACCESS_H
