#ifndef PB_STEPLOOPONFILES_H
#define PB_STEPLOOPONFILES_H

#include "ct_step/ct_stepbeginloop.h"
#include "ct_itemdrawable/ct_readeritem.h"
#include "ct_itemdrawable/ct_fileheader.h"

#include <QDirIterator>

class PB_StepLoopOnFiles : public CT_StepBeginLoop
{
    Q_OBJECT
    using SuperClass = CT_StepBeginLoop;

public:

    PB_StepLoopOnFiles();
    ~PB_StepLoopOnFiles() final;

    QString description() const final;

    CT_VirtualAbstractStep* createNewInstance() const final;

    void savePostSettings(SettingsWriterInterface& writer) const override;
    bool restorePostSettings(SettingsReaderInterface &reader) override;

protected:

    void fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog) final;

    void finalizePreSettings() final;

    bool postInputConfigure() final;

    void declareOutputModels(CT_StepOutModelStructureManager& manager) final;

    void compute() final;

private:
    CT_HandleOutResultGroup                         m_hOutResultLOF;
    CT_HandleOutStdGroup                            m_hOutRootGroupLOF;
    CT_HandleOutStdGroup                            m_hOutFileGroupLOF;
    CT_HandleOutSingularItem<CT_ReaderItem>         m_hOutReaderItemLOF;
    CT_HandleOutSingularItem<CT_FileHeader>         m_hOutFileHeaderLOF;

    CT_AbstractReader*                              mReader;

    /**
     * @brief Contains the classname of the selected reader
     */
    QString                                         m_readerSelectedUniqueName;

    /**
     * @brief Contains the path of the selected folder
     */
    QString                                         m_folderPath;

    /**
     * @brief Iterator to iterate over files in the compute methods
     */
    QDirIterator*                                   m_folderIterator;

    int countFiles() const;

    QDirIterator* createFilesIterator(const QString& dirPath) const;
};

#endif // PB_STEPLOOPONFILES_H
