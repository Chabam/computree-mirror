#ifndef PB_STEPLOOPONFILES_H
#define PB_STEPLOOPONFILES_H

#include "ct_step/ct_stepbeginloop.h"
#include "ct_reader/abstract/ct_abstractreader.h"
#include "ct_itemdrawable/ct_loopcounter.h"
#include "ct_itemdrawable/ct_standarditemgroup.h"

#include <QDirIterator>

class PB_StepLoopOnFiles: public CT_StepBeginLoop
{
    Q_OBJECT
    using SuperClass = CT_AbstractStep;
    typedef CT_StepBeginLoop SuperClass;

public:

    PB_StepLoopOnFiles();
    ~PB_StepLoopOnFiles();

    QString description() const;

    QString detailledDescription() const;

    QString getStepURL() const;

    CT_VirtualAbstractStep* createNewInstance() const final;

    void savePostSettings(SettingsWriterInterface& writer) const override;
    bool restorePostSettings(SettingsReaderInterface &reader) override;

protected:

    void fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog) final;

    bool postConfigure();

    void createOutResultModelListProtected(CT_OutResultModelGroup *firstResultModel);

    void compute(CT_ResultGroup *outRes, CT_StandardItemGroup *group);

private:

    /**
     * @brief Contains the classname of the selected reader
     */
    QString                                         m_readerSelectedClassName;

    /**
     * @brief Contains the path of the selected folder
     */
    QString                                         m_folderPath;

    /**
     * @brief Iterator to iterate over files in the compute methods
     */
    QDirIterator*                                   m_folderIterator;
};

#endif // PB_STEPLOOPONFILES_H
