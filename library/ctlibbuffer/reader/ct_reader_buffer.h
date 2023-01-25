#ifndef CT_READER_BUFFER_H
#define CT_READER_BUFFER_H

#include "ctlibbuffer/ctlibbuffer_global.h"
#include "ct_reader/abstract/ct_abstractreader.h"
#include "ct_itemdrawable/ct_scene.h"

/**
 * @brief Reader that can load a cti file (*.cti) that represent a PointCloud index list
 */
class CTLIBBUFFER_EXPORT CT_Reader_BUFFER : public CT_AbstractReader
{
    Q_OBJECT
    typedef CT_AbstractReader SuperClass;

public:
    CT_Reader_BUFFER(int subMenuLevel = 0);
    CT_Reader_BUFFER(const CT_Reader_BUFFER& other);
    ~CT_Reader_BUFFER();

    /**
     * @brief Returns a displayable name of the reader
     */
    QString displayableName() const override;

    /**
     * @brief Redefined to check the validity of the file. Returns false if the file is not compatible.
     */
    bool setFilePath(const QString& filepath) override;

    READER_ALL_COPY_IMP(CT_Reader_BUFFER)

protected:
    void internalDeclareOutputModels(CT_ReaderOutModelStructureManager& manager) override;

    CT_FileHeader* internalReadHeader(const QString &filepath, QString &error) const override;

    bool internalReadFile(CT_StandardItemGroup* group) override;

private:
    QString            m_data_path;
    QString            m_fileType;
    QList<FileBuffer>  m_fileBufferList;
    CT_AbstractReader* m_reader;
};

#endif // CT_READER_BUFFER_H
