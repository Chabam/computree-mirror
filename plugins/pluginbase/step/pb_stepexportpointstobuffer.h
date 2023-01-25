#ifndef PB_STEPEXPORTPOINTSTOBUFFER_H
#define PB_STEPEXPORTPOINTSTOBUFFER_H

#include "ct_step/abstract/ct_abstractstep.h"

#include "ct_exporter/abstract/ct_abstractexporter.h"
#include "ct_exporter/abstract/ct_abstractpiecebypieceexporter.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/abstract/ct_abstractareashape2d.h"

class PB_StepExportPointsToBuffer: public CT_AbstractStep
{
    Q_OBJECT
    using SuperClass = CT_AbstractStep;

public:
    PB_StepExportPointsToBuffer();
    ~PB_StepExportPointsToBuffer();

    QString description() const final;

    QString detailledDescription() const final;

    CT_VirtualAbstractStep* createNewInstance() const final;

protected:
    void declareInputModels(CT_StepInModelStructureManager& manager) final;

    void declareOutputModels(CT_StepOutModelStructureManager& manager) final;

    void compute() final;
private:
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

    int                     m_currentAreaIndex;
    double                  m_progressRangeForExporter;

private slots:
    void exporterProgressChanged(int p);
};

#endif // PB_STEPEXPORTPOINTSTOBUFFER_H
