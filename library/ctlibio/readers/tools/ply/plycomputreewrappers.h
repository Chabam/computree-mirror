#ifndef PLYCOMPUTREEWRAPPERS_H
#define PLYCOMPUTREEWRAPPERS_H

///////////////////////////////////////////////
/// This file will contains some wrappers
/// that can must be use by computree to read
/// a plys file and store readed values in
/// computree arrays.
///////////////////////////////////////////////

#include <QVBoxLayout>

#include "ct_point.h"
#include "ct_accessor/ct_pointaccessor.h"
#include "ct_cloudindex/registered/abstract/ct_abstractnotmodifiablecloudindexregisteredt.h"
#include "ct_colorcloud/ct_colorcloudstdvector.h"
#include "ct_normalcloud/ct_normalcloudstdvector.h"
#include "ct_cloud/ct_standardcloudstdvectort.h"
#include "ct_view/ct_abstractconfigurablewidget.h"

#include "readers/tools/ply/views/gplyreadconfiguration.h"

/**
 * @brief This is a wrapper to convert double values of a readed vertex
 *        in a ply file to a CT_PointData
 */
struct Ply_CT_Point_Wrapper {
    Ply_CT_Point_Wrapper()
    {
        globalIndex = 0;
        updateIt = true;
        accessor = nullptr;
    }

    /**
     * @brief Called by Ply_CT_PointCloud_Wrapper to set the accessor to use
     */
    void setAccessor(const CT_PointAccessor* acc)
    {
        accessor = const_cast<CT_PointAccessor*>(acc);
    }

    /**
     * @brief Called by Ply_CT_PointCloud_Wrapper so this wrapper know which point to modify
     * @param index : the global index of the point to modify in the global point cloud
     */
    void setCurrentGlobalIndex(const size_t& index)
    {
        if(globalIndex != index) {
            updateIt = true;
            globalIndex = index;
        }
    }

    /**
     * @brief Called by a ply reader when he want to modify the point
     * @param other : point that contains readed values
     */
    Ply_CT_Point_Wrapper& operator=(const Ply_CT_Point_Wrapper& other)
    {
        if(updateIt) {
            accessor->replacePointAt(globalIndex, other.p);
            updateIt = false;
        }

        return *this;
    }

    /**
     * @brief Called by a ply reader when he want to modify a specific
     *        coordinate of the point
     * @param index : index of the coordinate to modify
     */
    double& operator[](const size_t& index)
    {
        return p[qlonglong(index)];
    }

    bool                updateIt;
    size_t              globalIndex;
    CT_PointAccessor*   accessor;
    CT_Point            p;
};

/**
 * @brief This is a class that create a cloud of points and register it
 *        to the repository of computree. Also it will set the current
 *        global index of the point to modify.
 */
struct Ply_CT_PointCloud_Wrapper {
    typedef Ply_CT_Point_Wrapper value_type;

    Ply_CT_PointCloud_Wrapper()
    {
        pcir = CT_NMPCIR(nullptr);
        beginIndex = 0;
        point.setAccessor(&accessor);
    }

    /**
     * @brief Called by a ply reader when we must create the point cloud
     */
    void resize(const size_t& newSize)
    {
        if((newSize > 0)
                && pcir.isNull()) {
            pcir = PS_REPOSITORY->createNewPointCloud(newSize);
            beginIndex = pcir->abstractCloudIndex()->first();
        }
    }

    /**
     * @brief Called by a ply reader when he want to modify a point
     * @param index : local index of the point to modify in the "pcir" array
     * @return Returns a wrapper that can modify the computree point in the global point cloud
     */
    Ply_CT_Point_Wrapper& operator[](const size_t& index)
    {
        // set the current global index to the point so it can know which point to modify
        point.setCurrentGlobalIndex(beginIndex + index);

        return point;
    }

    CT_NMPCIR               pcir;
    size_t                  beginIndex;
    CT_PointAccessor        accessor;
    Ply_CT_Point_Wrapper    point;
};

/**
 * @brief This is a class that create a cloud of XXX and register it
 *        to the repository of computree. Also it will set the current
 *        global index of the point to modify.
 */
template<typename Cloud, typename ValueInCloud>
struct Ply_CT_GenericCloud_Wrapper {
    typedef ValueInCloud value_type;

    Ply_CT_GenericCloud_Wrapper()
    {
        cloud = NULL;
    }

    /**
     * @brief Called by a ply reader when we must create the point cloud
     */
    void resize(const size_t& newSize)
    {
        if(cloud == NULL)
            cloud = new Cloud(newSize);
    }

    /**
     * @brief Called by a ply reader when he want to modify a point
     * @param index : local index of the point to modify in the "pcir" array
     * @return Returns a wrapper that can modify the computree point in the global point cloud
     */
    inline value_type& operator[](const size_t& index)
    {
        return (*cloud)[index];
    }

    Cloud*                  cloud;
};

typedef Ply_CT_GenericCloud_Wrapper<CT_ColorCloudStdVector, CT_Color> Ply_CT_ColorCloud_Wrapper;
typedef Ply_CT_GenericCloud_Wrapper<CT_NormalCloudStdVector, CT_Normal> Ply_CT_NormalCloud_Wrapper;

// c++11
template<typename ValueInCloud>
using Ply_CT_ScalarCloud_Wrapper = Ply_CT_GenericCloud_Wrapper<CT_StandardCloudStdVectorT<ValueInCloud>, ValueInCloud>;

/**
 * @brief Wrapper to use the configuration widget of a ply file with computree
 */
class GPly_CT_ReadConfiguration_Wrapper : public CT_AbstractConfigurableWidget {
public:
    GPly_CT_ReadConfiguration_Wrapper(QWidget *parent = nullptr) : CT_AbstractConfigurableWidget(parent) {
        QVBoxLayout* layout = new QVBoxLayout(this);

        plyConfig = new GPlyReadConfiguration();
        layout->addWidget(plyConfig);

        setWindowTitle(plyConfig->windowTitle());
    }

    GPlyReadConfiguration* plyConfigurationWidget() const {
        return plyConfig;
    }

    virtual bool canAcceptIt(QString *err) const {
        if(plyConfig->isValid())
            return true;

        if(err != nullptr)
            *err = tr("Certains éléments ne sont pas complètement renseignés.");

        return false;
    }

    virtual bool isSettingsModified() const {
        return true;
    }

public slots:
    virtual bool updateElement(QString *err)  {
        return canAcceptIt(err);
    }

private:
    GPlyReadConfiguration* plyConfig;
};

#endif // PLYCOMPUTREEWRAPPERS_H
