#include "ct_abstractpiecebypieceexporter.h"

CT_AbstractPieceByPieceExporter::CT_AbstractPieceByPieceExporter() :
    mPointFilter(nullptr),
    mEdgeFilter(nullptr),
    mFaceFilter(nullptr),
    mFileCreated(false),
    mProgress(0),
    mMustCancel(false)
{
    setPointFilter(nullptr);
    setEdgeFilter(nullptr);
    setFaceFilter(nullptr);
}

bool CT_AbstractPieceByPieceExporter::createOrOpenFile()
{
    mMustCancel = false;
    setProgress(0);
    setErrorMessage(QString());

    if(mFileCreated)
        return internalOpenFileInAppendMode();

    if(internalCreateFile())
    {
        mFileCreated = true;
        return internalOpenFileInAppendMode();
    }

    return false;
}

void CT_AbstractPieceByPieceExporter::closeFile()
{
    internalCloseFile();
}

bool CT_AbstractPieceByPieceExporter::finalizeFile()
{
    if(mMustCancel)
        return true;

    const bool ok = internalFinalizeFile();
    mFileCreated = false;

    return ok;
}

void CT_AbstractPieceByPieceExporter::setPointFilter(CT_AbstractPieceByPieceExporter::PointFilter filter)
{
    mPointFilter = filter;

    if(mPointFilter == nullptr)
        mPointFilter = [](const size_t&, const CT_Point&) { return true; };
}

void CT_AbstractPieceByPieceExporter::setEdgeFilter(CT_AbstractPieceByPieceExporter::EdgeFilter filter)
{
    mEdgeFilter = filter;

    if(mEdgeFilter == nullptr)
        mEdgeFilter = [](const size_t&, const CT_Edge&) { return true; };
}

void CT_AbstractPieceByPieceExporter::setFaceFilter(CT_AbstractPieceByPieceExporter::FaceFilter filter)
{
    mFaceFilter = filter;

    if(mFaceFilter == nullptr)
        mFaceFilter = [](const size_t&, const CT_Face&) { return true; };
}

QString CT_AbstractPieceByPieceExporter::errorMessage() const
{
    return mErrorMessage;
}

void CT_AbstractPieceByPieceExporter::setErrorMessage(const QString& err)
{
    mErrorMessage = err;
}

void CT_AbstractPieceByPieceExporter::setProgress(int progress)
{
    if(mProgress != progress)
    {
        mProgress = progress;
        emit progressChanged(progress);
    }
}

void CT_AbstractPieceByPieceExporter::cancel()
{
    mMustCancel = true;
}
