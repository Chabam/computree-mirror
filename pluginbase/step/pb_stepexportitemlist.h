#ifndef PB_STEPEXPORTITEMLIST_H
#define PB_STEPEXPORTITEMLIST_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_exporter/abstract/ct_abstractexporter.h"
#include "ct_itemdrawable/ct_loopcounter.h"

/*!
 * \class PB_StepExportItemList
 * \ingroup Steps_PB
 * \brief <b>Exporter une liste d'items à l'aide d'un exporter.</b>
 *
 * No detailled description for this step
 *
 *
 */

class PB_StepExportItemList: public CT_AbstractStep
{
    Q_OBJECT
    using SuperClass = CT_AbstractStep;

public:

    PB_StepExportItemList();

    QString description() const final;

    QString detailledDescription() const final;

    void saveInputSettings(SettingsWriterInterface& writer) const final;
    bool restoreInputSettings(SettingsReaderInterface &reader) final;

    CT_VirtualAbstractStep* createNewInstance() const final;

protected:

    void fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog) final;

    void finalizePreSettings();

    void declareInputModels(CT_StepInModelStructureManager& manager) final;

    void fillPostInputConfigurationDialog(CT_StepConfigurableDialog* postInputConfigDialog) final;

    void declareOutputModels(CT_StepOutModelStructureManager& manager) final;

    void compute() final;

private:

    // Step parameters
    QStringList _dir;
    QString _prefixFileName;

    /**
     * @brief Contains the key of the selected exporter
     */
    QString m_exporterSelectedKey;

    CT_AbstractExporter*    mExporter;
};

#endif // PB_STEPEXPORTITEMLIST_H
