#ifndef PB_STEPLOOPONFILESSETS_H
#define PB_STEPLOOPONFILESSETS_H

#include "ct_step/ct_stepbeginloop.h"
#include "ct_reader/abstract/ct_abstractreader.h"
#include "ct_itemdrawable/ct_loopcounter.h"
#include "ct_itemdrawable/ct_standarditemgroup.h"

#include <QDirIterator>
#include <QFileInfo>

class PB_StepLoopOnFileSets: public CT_StepBeginLoop
{
    Q_OBJECT
    using SuperClass = CT_AbstractStep;
    typedef CT_StepBeginLoop SuperClass;

public:

    PB_StepLoopOnFileSets();

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
     * @brief Contains the filepath of the selected file with sets
     */
    QString                                         m_setsFilePath;

    /**
     * @brief Contains the filepath of the selected file with sets
     */
    QString                                         m_exampleFileFolderPath;

    QList<QString>                                  _setKeys;
    QMultiMap<QString, QString>                     _sets;
};

#endif // PB_STEPLOOPONFILESSETS_H
