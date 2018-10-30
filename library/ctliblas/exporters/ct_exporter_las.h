#ifndef CT_EXPORTER_LAS_H
#define CT_EXPORTER_LAS_H

#include "ctliblas/ctliblas_global.h"
#include "ctliblas/itemdrawable/las/ct_stdlaspointsattributescontainer.h"
#include "ctliblas/tools/las/abstract/ct_abstractlaspointformat.h"

// from pluginshared
#include "ct_exporter/abstract/ct_abstractexporterattributesselection.h"
#include "ct_tools/itemdrawable/ct_itemdrawablecollectionbuildert.h"

class CTLIBLAS_EXPORT CT_Exporter_LAS : public CT_AbstractExporterAttributesSelection
{
    Q_OBJECT
    typedef CT_AbstractExporterAttributesSelection SuperClass;

public:
    CT_Exporter_LAS();
    CT_Exporter_LAS(const CT_Exporter_LAS& other);
    ~CT_Exporter_LAS();

    QString getExporterCustomName() const override;

    CT_StepsMenu::LevelPredefined getExporterSubMenuName() const override;

    void init() override;

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list) override;
    bool setPointsToExport(const QList<CT_AbstractCloudIndex*> &list) override;

    void saveSettings(SettingsWriterInterface& writer) const override;
    bool restoreSettings(SettingsReaderInterface& reader) override;

    bool canExportPieceByPiece() const override;
    bool createExportFileForPieceByPieceExport() override;
    bool exportOnePieceOfDataToFile() override;
    bool finalizePieceByPieceExport() override;

    CT_EXPORTER_DECL_COPY(CT_Exporter_LAS)

protected:
    QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > createBuilders() const override;
    bool useSelection(const CT_ItemDrawableHierarchyCollectionWidget *selectorWidget) override;

    bool protectedExportToFile() override;

private:
    typedef CT_ItemDrawableCollectionBuilderT<CT_StdLASPointsAttributesContainer> LasAttributesBuilder;

    struct HeaderBackup {
        HeaderBackup() : header(NULL), nFiles(0), toolsFormat(NULL) {}
        ~HeaderBackup() { delete header; delete toolsFormat; }

        bool isHeaderSameForExport(const CT_LASHeader* header) const;
        QString previousFilePath() const;
        QString currentFilePath() const;
        QString generateFilePath(int n) const;

        CT_LASHeader*               header;
        QString                     dirPath;
        int                         nFiles;
        CT_AbstractLASPointFormat*  toolsFormat;
    };

    CT_StdLASPointsAttributesContainer*                                     m_lasContainer;
    CT_OutAbstractSingularItemModel*                                        m_lasContainerModel;
    QList<HeaderBackup*>                                                    m_headers;
    QString                                                                 m_baseFilePath;

    /**
     * @brief Set NULL to m_lasContainer and m_lasContainerModel
     */
    void clearSelection();

    /**
     * @brief Create the header
     * @param rn : the points attributes that represent for each point the "return number"
     * @param indexes : the list of all indexes that will be exported
     * @param nPoints : will be filled with the total number of points to write (sum of size of index in indexes)
     * @return NULL if it was an error otherwiser the header created (don't forget to delete it from memory)
     */
    CT_LASHeader* createHeader(const CT_AbstractPointAttributesScalar *rn,
                               const QList<CT_AbstractPointCloudIndex *> &indexes,
                               size_t &nPoints) const;

    /**
     * @brief Write the header of the file
     * @param stream : the data stream of the file (it will be seek to 0 automatically and set to LittleEndian)
     * @param header : the header to write
     */
    bool writeHeader(QDataStream& stream,
                     CT_LASHeader *header);

    /**
     * @brief Return the point data format to use (check m_lasContainer to know where attributes is in the container and select the appropriate format)
     */
    int getPointDataFormat() const;

    /**
     * @brief Return the length of a point in bytes (use the method "getPointDataFormat" if optFormat == -1, otherwise pass the format to this method)
     */
    quint16 getPointDataLength(const int &optFormat = -1) const;

    /**
     * @brief Create a point data format (tools to export)
     */
    CT_AbstractLASPointFormat* createPointDataFormat(const int &optFormat = -1) const;

    /**
     * @brief Search an header in the list of header backuped and if has found one return the backup that match with it
     */
    HeaderBackup* createOrGetHeaderBackupForHeader(const CT_LASHeader* header) const;

    /**
     * @brief When you have finished to write the file call this method to save or delete the header backup
     */
    void saveOrDeleteHeaderBackup(HeaderBackup* backup, CT_LASHeader* header, bool writeOfFileIsOk);

    /**
     * @brief Clear all backuped headers
     */
    void clearAllBackupedHeaders();
};

#endif // CT_EXPORTER_LAS_H
