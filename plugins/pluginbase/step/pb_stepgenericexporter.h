#ifndef PB_STEPGENERICEXPORTER_H
#define PB_STEPGENERICEXPORTER_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_exporter/abstract/ct_abstractexporter.h"

/*!
 * \class PB_StepGenericExporter
 * \ingroup Steps_PB
 * \brief <b>Generic exporter</b>
 *
 * This step can export any ItemDrawable by the way of
 * loaded exporter plugins.
 *
 */
class PB_StepGenericExporter : public CT_AbstractStep
{
    Q_OBJECT
    using SuperClass = CT_AbstractStep;

public:

    PB_StepGenericExporter(CT_AbstractExporter *exporter);
    ~PB_StepGenericExporter() final;

    const CT_AbstractExporter* exporter() const;

    /**
     * @brief Redefined to return the unique name of the exporter
     */
    QString name() const final;

    /**
     * @brief Redefined to return the displayable name of the exporter
     */
    QString displayableName() const final;

    /**
     * @brief Redefined to return the tooltip of the exporter
     */
    QString description() const final;

    /**
     * @brief Redefined to return the tooltip of the exporter or, if empty, type of files
     *        that can be loaded by the exporter
     */
    QString detailledDescription() const final;

    void saveInputSettings(SettingsWriterInterface& writer) const final;
    bool restoreInputSettings(SettingsReaderInterface &reader) final;

    void savePostSettings(SettingsWriterInterface& writer) const final;
    bool restorePostSettings(SettingsReaderInterface &reader) final;

    CT_VirtualAbstractStep* createNewInstance() const final;

protected:

    QString createExportExtensionString() const;

    void fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog) final;

    void declareInputModels(CT_StepInModelStructureManager& manager) final;

    bool postInputConfigure() final;

    void declareOutputModels(CT_StepOutModelStructureManager& manager) final;

    void compute();

private:
    CT_AbstractExporter *_exporter;
    QString             _exportPath;
    QString             _exportFilename;

    bool                _adaptative;
    int                 _multipleExport;
    int                 _multipleExportChoice;

private slots:
    void exportProgressChanged(int progress);
};

#endif // PB_STEPGENERICEXPORTER_H
