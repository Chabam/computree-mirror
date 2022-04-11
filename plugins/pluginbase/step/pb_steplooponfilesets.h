#ifndef PB_STEPLOOPONFILESSETS_H
#define PB_STEPLOOPONFILESSETS_H

#include "ct_step/ct_stepbeginloop.h"
#include "ct_itemdrawable/ct_readeritem.h"
#include "ct_itemdrawable/ct_fileheader.h"
#include "ct_itemdrawable/ct_itemattributelist.h"

class PB_StepLoopOnFileSets: public CT_StepBeginLoop
{
    Q_OBJECT
    using SuperClass = CT_StepBeginLoop;

public:
    PB_StepLoopOnFileSets();
    ~PB_StepLoopOnFileSets();

    QString description() const final;
    QString detailledDescription() const final;

    CT_VirtualAbstractStep* createNewInstance() const final;

    void savePostSettings(SettingsWriterInterface& writer) const final;
    bool restorePostSettings(SettingsReaderInterface &reader) final;

protected:

    void fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog) final;

    void finalizePreSettings() final;

    bool postInputConfigure() final;

    void declareOutputModels(CT_StepOutModelStructureManager& manager) final;

    void compute() final;

private:

    struct Set {
        QString     mName;
        QStringList mFilesPath;
    };

    CT_HandleOutResultGroup                         m_hOutResultLOFS;
    CT_HandleOutStdGroup                            m_hOutRootGroupLOFS;
    CT_HandleOutStdGroup                            m_hOutSetGroupLOFS;
    CT_HandleOutSingularItem<CT_ItemAttributeList>  m_hOutSetItemLOFS;
    CT_HandleOutStdStringItemAttribute              m_hOutSetItemAttributeLOFS;
    CT_HandleOutStdGroup                            m_hOutFileGroupLOFS;
    CT_HandleOutSingularItem<CT_ReaderItem>         m_hOutReaderItemLOFS;
    CT_HandleOutSingularItem<CT_FileHeader>         m_hOutFileHeaderLOFS;

    CT_AbstractReader*                              mReader;

    /**
     * @brief Contains the classname of the selected reader
     */
    QString                                         m_readerSelectedUniqueName;

    /**
     * @brief Contains the filepath of the selected file with sets
     */
    QString                                         m_setsFilePath;

    /**
     * @brief Contains the filepath of the selected file with sets
     */
    QString                                         m_exampleFileFolderPath;

    QList<Set>                                      _sets;
    int                                             _nFiles;

    static QList<Set> ReadSetsInFile(const QString& filepath, const QString& folderPath, CT_AbstractReader* reader, int* countFile = nullptr);
};

#endif // PB_STEPLOOPONFILESSETS_H
