#include "dm_asyncoperation.h"

DM_AsyncOperation::DM_AsyncOperation()
{
    m_progressDialog = nullptr;
}

void DM_AsyncOperation::setProgressDialog(const IMainProgressDialog *dialog)
{
    if(m_progressDialog != nullptr)
        disconnect(m_progressDialog, nullptr, this, nullptr);

    m_progressDialog = (IMainProgressDialog*)dialog;

    if(m_progressDialog != nullptr)
        connect(m_progressDialog, SIGNAL(cancel()), this, SIGNAL(cancel()), Qt::QueuedConnection);
}

IMainProgressDialog* DM_AsyncOperation::progressDialog() const
{
    return m_progressDialog;
}

void DM_AsyncOperation::setProgress(const int &progress)
{
    m_progressDialog->setValue(progress);
}

void DM_AsyncOperation::setProgressRange(const int &min, const int &max)
{
    m_progressDialog->setProgressRange(min, max);
}
