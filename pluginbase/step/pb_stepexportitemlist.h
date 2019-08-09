#ifndef PB_STEPEXPORTITEMLIST_H
#define PB_STEPEXPORTITEMLIST_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_exporter/abstract/ct_abstractexporter.h"

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

    QString description() const;

    QString detailledDescription() const;

    QString getStepURL() const;

    CT_VirtualAbstractStep* createNewInstance() const final;

protected:

    void declareInputModels(CT_StepInModelStructureManager& manager) final;

    // CT_AbstractStep non obligatoire :
//    bool configureInputResult();
//    bool configureExporter();
//    void configureExporterFromModel();

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
};

#endif // PB_STEPEXPORTITEMLIST_H
