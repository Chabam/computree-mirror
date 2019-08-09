#ifndef PB_STEPLOADFILEBYNAME_H
#define PB_STEPLOADFILEBYNAME_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_reader/abstract/ct_abstractreader.h"
#include "ct_itemdrawable/ct_loopcounter.h"
#include "ct_itemdrawable/ct_standarditemgroup.h"

#include <QDirIterator>
#include <QFileInfo>

class PB_StepLoadFileByName: public CT_AbstractStep
{
    Q_OBJECT
    using SuperClass = CT_AbstractStep;
    typedef CT_AbstractStep SuperClass;

public:

    PB_StepLoadFileByName();

    QString description() const;

    QString detailledDescription() const;

    QString getStepURL() const;

    CT_VirtualAbstractStep* createNewInstance() const final;

    void savePostSettings(SettingsWriterInterface &writer) const override;
    bool restorePostSettings(SettingsReaderInterface &reader) override;

protected:

    void fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog) final;

//    void fillPostInputConfigurationDialog(CT_StepConfigurableDialog* postInputConfigDialog) final;

    bool postConfigure();

    void declareInputModels(CT_StepInModelStructureManager& manager) final;

    void declareOutputModels(CT_StepOutModelStructureManager& manager) final;

    void compute() final;

private:

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
