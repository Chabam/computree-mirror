#ifndef DM_AMKGLEASYACCESS_H
#define DM_AMKGLEASYACCESS_H

#include "interfaces/ieasyaccess.h"
#include "dm_amkglcoordinatesystemmanager.h"
#include "dm_amkglglobalpointscloudmanager.h"
#include "dm_amkglglobaledgescloudmanager.h"
#include "dm_amkglglobalfacescloudmanager.h"

class DM_AMKglEasyAccess : public IEasyAccess, public IAMKglLog
{
public:
    DM_AMKglEasyAccess();

    IGlobalPointCloudManager* getGlobalPointCloudManager() const;
    IGlobalEdgeCloudManager* getGlobalEdgeCloudManager() const;
    IGlobalFaceCloudManager* getGlobalFaceCloudManager() const;
    ICoordinateSystemManager* getCoordinateSystemManager() const;
    IAMKglLog* getLogger() const;

    void addDebugMessage(const QString& mess);
    void addInfoMessage(const QString& mess);
    void addWarningMessage(const QString& mess);
    void addErrorMessage(const QString& mess);

private:
    DM_AMKglCoordinateSystemManager     m_csm;
    DM_AMKglGlobalPointsCloudManager    m_gpclm;
    DM_AMKglGlobalEdgesCloudManager     m_eclm;
    DM_AMKglGlobalFacesCloudManager     m_fclm;
};

#endif // DM_AMKGLEASYACCESS_H
