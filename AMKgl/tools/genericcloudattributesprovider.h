#ifndef GENERICCLOUDATTRIBUTESPROVIDER_H
#define GENERICCLOUDATTRIBUTESPROVIDER_H

#include "amkglapp.h"
#include "icloudattributesprovider.h"

/**
 * @brief Provider for attributes of T (points, faces, edges)
 *        ICloudManager must be of type IGlobalPointCloudManager or IGlobalEdgeCloudManager or IGlobalFaceCloudManager
 */
template<typename ICloudManager>
class GenericCloudAttributesProvider : public ICloudAttributesProvider {
public:
    typedef GenericCloudAttributesProvider<ICloudManager> ThisClass;

    GenericCloudAttributesProvider() {
        m_globalCloudManager = AMKglEA->getGlobalCloudManager<ICloudManager>();

        m_colorCloudRegistered = GlobalColorCloudRegistered(NULL);
        m_normalCloudRegistered = GlobalNormalCloudRegistered(NULL);
        m_infoCloudRegistered = GlobalInfoCloudRegistered(NULL);

        m_colorCloud = NULL;
        m_normalCloud = NULL;
        m_infoCloud = NULL;
    }

    virtual ~GenericCloudAttributesProvider() {}

    /**
     * @brief Returns the cloud that store colors of points. If it wasn't created it will be before returned.
     */
    GlobalColorCloud* createOrGetColorCloud()
    {
        if(m_colorCloudRegistered.isNull()) {
            ThisClass* thisObject = const_cast<ThisClass*>(this);
            thisObject->m_colorCloudRegistered = m_globalCloudManager->createNewColorCloudAndRegisterIt(thisObject->m_colorCloud);
        }

        return m_colorCloud;
    }

    /**
     * @brief Returns the cloud that store colors of normals. If it wasn't created it will be before returned.
     */
    GlobalNormalCloud* createOrGetNormalCloud()
    {
        if(m_normalCloudRegistered.isNull()) {
            ThisClass* thisObject = const_cast<ThisClass*>(this);
            thisObject->m_normalCloudRegistered = m_globalCloudManager->createNewNormalCloudSyncedAndRegisterIt(thisObject->m_normalCloud);
        }

        return m_normalCloud;
    }

    /**
     * @brief Returns the cloud that store informations of points (visibility, etc...). If it wasn't created it will be before returned.
     */
    GlobalInfoCloud* createOrGetInfoCloud()
    {
        if(m_infoCloudRegistered.isNull()) {
            ThisClass* thisObject = const_cast<ThisClass*>(this);
            thisObject->m_infoCloudRegistered = m_globalCloudManager->createNewInfoCloudSyncedAndRegisterIt(thisObject->m_infoCloud);
        }

        return m_infoCloud;
    }


    /**
     * @brief Returns the cloud that store colors of points
     */
    GlobalColorCloud* getColorCloud() const { return m_colorCloud; }

    /**
     * @brief Returns the cloud that store colors of normals
     */
    GlobalNormalCloud* getNormalCloud() const { return m_normalCloud; }

    /**
     * @brief Returns the cloud that store informations of points (visibility, etc...)
     */
    GlobalInfoCloud* getInfoCloud() const { return m_infoCloud; }

    /**
     * @brief Returns the cloud that store colors of points
     */
    GlobalColorCloudRegistered createOrGetColorCloudRegistered()
    {
        createOrGetColorCloud();
        return m_colorCloudRegistered;
    }

    /**
     * @brief Returns the cloud that store colors of normals
     */
    GlobalNormalCloudRegistered createOrGetNormalCloudRegistered()
    {
        createOrGetNormalCloud();
        return m_normalCloudRegistered;
    }

    /**
     * @brief Returns the cloud that store informations of points (visibility, etc...)
     */
    GlobalInfoCloudRegistered createOrGetInfoCloudRegistered()
    {
        createOrGetInfoCloud();
        return m_infoCloudRegistered;
    }

private:

    ICloudManager*                      m_globalCloudManager;

    /**
     * @brief Color cloud
     */
    GlobalColorCloudRegistered          m_colorCloudRegistered;
    GlobalColorCloud*                   m_colorCloud;

    /**
     * @brief Normal cloud
     */
    GlobalNormalCloudRegistered         m_normalCloudRegistered;
    GlobalNormalCloud*                  m_normalCloud;

    /**
     * @brief Information cloud
     */
    GlobalInfoCloudRegistered           m_infoCloudRegistered;
    GlobalInfoCloud*                    m_infoCloud;
};

#endif // GENERICCLOUDATTRIBUTESPROVIDER_H
