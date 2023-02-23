#ifndef PB_STEPCREATEREADERLIST_H
#define PB_STEPCREATEREADERLIST_H

#include "ct_step/abstract/ct_abstractstepcanbeaddedfirst.h"
#include "ct_itemdrawable/ct_fileheader.h"

/**
 * @brief Step that can use any type of reader to load multiple files (of same type).
 */
class PB_StepCreateReaderList: public CT_AbstractStepCanBeAddedFirst
{
    Q_OBJECT
    using SuperClass = CT_AbstractStepCanBeAddedFirst;

public:
    PB_StepCreateReaderList();
    ~PB_StepCreateReaderList() final;

    QString description() const final;

    QString detailledDescription() const final;

    QString outputDescription() const override;
    QString detailsDescription() const override;

    void savePostSettings(SettingsWriterInterface& writer) const final;
    bool restorePostSettings(SettingsReaderInterface &reader) final;

    CT_VirtualAbstractStep* createNewInstance() const final;

protected:
    void fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog) final;

    void finalizePreSettings() final;

    void declareInputModels(CT_StepInModelStructureManager& manager) final;

    bool postInputConfigure() final;

    void declareOutputModels(CT_StepOutModelStructureManager& manager) final;

    void compute() final;

private:
    CT_HandleOutResultGroup                 m_hOutResult;
    CT_HandleOutStdGroup                    m_hOutRootGroup;
    CT_HandleOutStdGroup                    m_hOutFileGroup;
    CT_HandleOutSingularItem<CT_FileHeader> m_hOutFileHeader;

    CT_AbstractReader*                      mReader;

    /**
     * @brief Contains the classname of the selected reader
     */
    QString         m_readerSelectedUniqueName;
    int             m_choiceMode;

    /**
     * @brief Contains the list of filepath of all files selected
     */
    QStringList     m_filepathCollection;
};

#endif // PB_STEPCREATEREADERLIST_H
