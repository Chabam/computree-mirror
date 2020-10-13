#ifndef PB_STEPLOADFILEBYNAME_H
#define PB_STEPLOADFILEBYNAME_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_reader/abstract/ct_abstractreader.h"

#include "ct_itemdrawable/ct_readeritem.h"
#include "ct_itemdrawable/ct_fileheader.h"

class PB_StepLoadFileByName : public CT_AbstractStep
{
    Q_OBJECT
    using SuperClass = CT_AbstractStep;

public:
    PB_StepLoadFileByName();
    ~PB_StepLoadFileByName() final;

    QString description() const final;

    QString detailledDescription() const final;

    CT_VirtualAbstractStep* createNewInstance() const final;

    void savePostSettings(SettingsWriterInterface &writer) const override;
    bool restorePostSettings(SettingsReaderInterface &reader) override;

protected:

    void fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog) final;

    void declareInputModels(CT_StepInModelStructureManager& manager) final;

    bool postInputConfigure() final;

    void declareOutputModels(CT_StepOutModelStructureManager& manager) final;

    void compute() final;

private:
    CT_HandleInResultGroupCopy<>                            m_hInResult;
    CT_HandleInStdZeroOrMoreGroup                           m_hInRootGroup;
    CT_HandleInStdGroup<>                                   m_hInGroup;
    CT_HandleInSingularItem<>                               m_hInItem;
    CT_HandleInStdItemAttribute<CT_AbstractCategory::ANY>   m_hInItemAttribute;

    CT_HandleOutSingularItem<CT_ReaderItem>                 m_hOutReaderItem;
    CT_HandleOutSingularItem<CT_FileHeader>                 m_hOutFileHeader;

    CT_AbstractReader*                                      mReader;

    /**
     * @brief Contains the classname of the selected reader
     */
    QString                                         m_readerSelectedClassName;

    /**
     * @brief Contains the path of the selected folder
     */
    QString                                         m_folderPath;


};

#endif // PB_STEPLOADFILEBYNAME_H
