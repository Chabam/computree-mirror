#ifndef CT_LAZPIECEBYPIECEPRIVATEEXPORTER_H
#define CT_LAZPIECEBYPIECEPRIVATEEXPORTER_H

#include "ctliblaz/exporters/ct_exporter_laz.h"
#include "ctliblaz/readers/ct_reader_laz.h"
#include "ct_exporter/abstract/ct_abstractpiecebypieceexporter.h"

class CT_LAZPieceByPiecePrivateExporter final : public CT_AbstractPieceByPieceExporter
{
    Q_OBJECT

public:
    ~CT_LAZPieceByPiecePrivateExporter() final;

protected:
    bool internalCreateFile() final;

    bool internalOpenFileInAppendMode() final;

    bool initializeHeader(const quint8 format, const quint16 pointDataLength);

    void computePointForHeader(const CT_AbstractPointAttributesScalar* returnNumberValues, const size_t& gi, const CT_Point& point);

    bool finalizeHeaderAndWritePoints();

    void internalCloseFile() final;

    bool internalFinalizeFile() final;

private:
    friend class CT_Exporter_LAZ;
    using HandleItemType = CT_Exporter_LAZ::HandleItemType;

    struct HeaderBackup {
        HeaderBackup() : header(nullptr), nFiles(0) {}
        ~HeaderBackup() { delete header; }

        bool isHeaderSameForExport(const CT_LAZHeader* header) const;
        QString previousFilePath() const;
        QString currentFilePath() const;
        QString generateFilePath(int n) const;

        const CT_LAZHeader*         header;
        QString                     dirPath;
        int                         nFiles;
    };

    CT_Exporter_LAZ&                            mExporter;
    QString                                     mFilePath;
    QString                                     mBaseFilePath;
    QList<HeaderBackup*>                        mHeaders;

    bool                                        mReturnNumberSet;
    CT_LAZHeader*                               mHeader;

    CT_PointCloudIndexVector                    mPointsToWrite;

    CT_LAZPieceByPiecePrivateExporter(CT_Exporter_LAZ& exporter, const QString& filePath);

    /**
     * @brief Write the header of the file
     * @param header : the header to write
     * @param zipHeader : header object for LASZIP
     */
    bool writeHeader(CT_LAZHeader *header,
                     laszip_header &zipHeader);

    /**
     * @brief Search an header in the list of header backuped and if has found one return the backup that match with it
     */
    HeaderBackup* createOrGetHeaderBackupForHeader(const CT_LAZHeader* header) const;

    /**
     * @brief When you have finished to write the file call this method to save or delete the header backup
     */
    void saveOrDeleteHeaderBackup(HeaderBackup* backup, CT_LAZHeader* header, bool writeOfFileIsOk);

    /**
     * @brief Clear all backuped headers
     */
    void clearAllBackupedHeaders();
};

#endif // CT_LAZPIECEBYPIECEPRIVATEEXPORTER_H
