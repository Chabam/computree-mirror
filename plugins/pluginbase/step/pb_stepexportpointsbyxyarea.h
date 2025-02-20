#ifndef PB_STEPEXPORTPOINTSBYXYAREA_H
#define PB_STEPEXPORTPOINTSBYXYAREA_H

#include "ct_step/abstract/ct_abstractstep.h"

#include "ct_exporter/abstract/ct_abstractexporter.h"
#include "ct_exporter/abstract/ct_abstractpiecebypieceexporter.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/abstract/ct_abstractareashape2d.h"

class PB_StepExportPointsByXYArea: public CT_AbstractStep
{
    Q_OBJECT
    using SuperClass = CT_AbstractStep;

public:
    PB_StepExportPointsByXYArea();
    ~PB_StepExportPointsByXYArea() final;

    QString description() const final;

    QString detailledDescription() const final;

    void savePostSettings(SettingsWriterInterface& writer) const final;
    bool restorePostSettings(SettingsReaderInterface &reader) final;

    CT_VirtualAbstractStep* createNewInstance() const final;

protected:
    void fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog) final;
    void finalizePreSettings() final;

    void declareInputModels(CT_StepInModelStructureManager& manager) final;

    void fillPostInputConfigurationDialog(CT_StepConfigurableDialog* postInputConfigDialog) final;

    bool postInputConfigure();

    bool configureExporter();

    void declareOutputModels(CT_StepOutModelStructureManager& manager) final;

    void compute() final;
private:

    struct AreaData
    {
        AreaData(CT_AreaShape2DData* area, CT_AbstractPieceByPieceExporter* exporter)
        {
            _area = area;
            _exporter = exporter;
        }

        ~AreaData()
        {
            delete _area;
            delete _exporter;
        }

        CT_AreaShape2DData*                 _area;
        CT_AbstractPieceByPieceExporter*    _exporter;
    };

    CT_HandleInResultGroup<>                                        m_hInCounterResult;
    CT_HandleInStdZeroOrMoreGroup                                   m_hInZeroOrMoreGroupCounter;
    CT_HandleInStdGroup<>                                           m_hInCounterRootGroup;
    CT_HandleInSingularItem<CT_LoopCounter>                         m_hInLoopCounter;

    CT_HandleInResultGroup<>                                        m_hInAreaResult;
    CT_HandleInStdZeroOrMoreGroup                                   m_hInZeroOrMoreGroupArea;
    CT_HandleInStdGroup<>                                           m_hInAreaRootGroup;
    CT_HandleInSingularItem<CT_AbstractAreaShape2D>                 m_hInArea;
    CT_HandleInStdItemAttribute<CT_AbstractCategory::ANY, 0>        m_hInAreaAttribute; // optionnal

    // Step parameters
    QStringList _dir;
    QString _suffixFileName;

    bool    _nameByCoordinates;
    double  _nameByCoordinatesOffset;

    /**
     * @brief Contains the classname of the selected reader
     */
    QString                                         m_exporterSelectedClassName;

    /**
     * @brief Contains the classname of the last selected reader (if "m_exporterSelectedClassName" change we can compare to this value)
     */
    QString                                         m_lastExporterSelectedClassName;

    /**
     * @brief A copy of the exporter
     */
    CT_AbstractExporter*                            m_exporter;

    QList<AreaData*>                _areas;
    int                     m_currentAreaIndex;
    double                  m_progressRangeForExporter;

private slots:
    void exporterProgressChanged(int p);

};

#endif // PB_STEPEXPORTPOINTSBYXYAREA_H
