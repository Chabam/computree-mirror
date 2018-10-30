#include "dm_amkgleasyaccess.h"

#include "dm_guimanager.h"

DM_AMKglEasyAccess::DM_AMKglEasyAccess()
{
}

IGlobalPointCloudManager* DM_AMKglEasyAccess::getGlobalPointCloudManager() const
{
    return (IGlobalPointCloudManager*)&m_gpclm;
}

IGlobalEdgeCloudManager* DM_AMKglEasyAccess::getGlobalEdgeCloudManager() const
{
    return (IGlobalEdgeCloudManager*)&m_eclm;
}

IGlobalFaceCloudManager* DM_AMKglEasyAccess::getGlobalFaceCloudManager() const
{
    return (IGlobalFaceCloudManager*)&m_fclm;
}

ICoordinateSystemManager* DM_AMKglEasyAccess::getCoordinateSystemManager() const
{
    return (ICoordinateSystemManager*)&m_csm;
}

IAMKglLog* DM_AMKglEasyAccess::getLogger() const
{
    return const_cast<DM_AMKglEasyAccess*>(this);
}

void DM_AMKglEasyAccess::addWarningMessage(const QString &mess)
{
    GUI_LOG->addWarningMessage(LogInterface::gui, QObject::tr("AMKGL : %1").arg(mess));
}

void DM_AMKglEasyAccess::addErrorMessage(const QString &mess)
{
    GUI_LOG->addErrorMessage(LogInterface::gui, QObject::tr("AMKGL : %1").arg(mess));
}

void DM_AMKglEasyAccess::addDebugMessage(const QString &mess)
{
    Q_UNUSED(mess)
    // don't show debug message
    //GUI_LOG->addDebugMessage(LogInterface::gui, QObject::tr("AMKGL : %1").arg(mess));
}

void DM_AMKglEasyAccess::addInfoMessage(const QString &mess)
{
    Q_UNUSED(mess)
    // don't show info message
    //GUI_LOG->addInfoMessage(LogInterface::gui, QObject::tr("AMKGL : %1").arg(mess));
}
