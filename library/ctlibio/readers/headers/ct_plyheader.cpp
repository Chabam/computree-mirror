#include "ct_plyheader.h"

CT_DEFAULT_IA_INIT(CT_PLYHeader)

CT_PLYHeader::CT_PLYHeader() : CT_FileHeader()
{
}

CT_PLYHeader::CT_PLYHeader(const CT_OutAbstractSingularItemModel *model,
                           const CT_AbstractResult *result) : CT_FileHeader(model, result)
{
}

CT_PLYHeader::CT_PLYHeader(const QString &modelName,
                           const CT_AbstractResult *result) : CT_FileHeader(modelName, result)
{
}

void CT_PLYHeader::setHeader(const PlyHeader &h)
{
    m_header = h;
    setFile(m_header.getFilePath());
}

PlyHeader CT_PLYHeader::getHeader() const
{
    return m_header;
}

QString CT_PLYHeader::getDisplayableFileFormat() const
{
    return m_header.getDisplayableFileFormat();
}

float CT_PLYHeader::getFileFormatVersion() const
{
    return m_header.getFileFormatVersion();
}

int CT_PLYHeader::getNumberOfElements() const
{
    return m_header.getElements().size();
}

int CT_PLYHeader::getNumberOfProperties() const
{
    int n = 0;

    QList<PlyElement> elements = m_header.getElements();

    foreach (const PlyElement& el, elements) {
        n += el.getProperties().size();
    }

    return n;
}

bool CT_PLYHeader::isValid() const
{
    return m_header.isValid();
}

CT_AbstractItemDrawable* CT_PLYHeader::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);

    CT_PLYHeader *cpy = new CT_PLYHeader((CT_OutAbstractSingularItemModel*) model, result);
    cpy->setFile(_fileInfo.absoluteFilePath());
    cpy->setHeader(m_header);

    return cpy;
}
