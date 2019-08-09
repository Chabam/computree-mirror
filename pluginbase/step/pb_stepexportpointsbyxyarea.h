#ifndef PB_STEPEXPORTPOINTSBYXYAREA_H
#define PB_STEPEXPORTPOINTSBYXYAREA_H

#include "ct_step/abstract/ct_abstractstep.h"

#include "ct_exporter/abstract/ct_abstractexporter.h"
#include "ct_itemdrawable/abstract/ct_abstractareashape2d.h"

class PB_StepExportPointsByXYArea: public CT_AbstractStep
{
    Q_OBJECT
    using SuperClass = CT_AbstractStep;
    typedef CT_AbstractStep SuperClass;

public:

    PB_StepExportPointsByXYArea();

    ~PB_StepExportPointsByXYArea();

    QString description() const;

    QString detailledDescription() const;

    QString getStepURL() const;

    void savePostSettings(SettingsWriterInterface& writer) const override;
    bool restorePostSettings(SettingsReaderInterface &reader) override;

    CT_VirtualAbstractStep* createNewInstance() const final;

protected:

    void declareInputModels(CT_StepInModelStructureManager& manager) final;

    bool postConfigure();
    void refreshExporterToUse();
    bool configureExporter();

    void fillPostInputConfigurationDialog(CT_StepConfigurableDialog* postInputConfigDialog) final;

    void declareOutputModels(CT_StepOutModelStructureManager& manager) final;

    void compute() final;

private:

    struct AreaData
    {
        AreaData(CT_AreaShape2DData* area, CT_AbstractExporter* exporter)
        {
            _area = area;
            _exporter = exporter;
            _cloudIndex = nullptr;
        }

        ~AreaData()
        {
            delete _area;
            delete _exporter;
        }

        void setPointCloudIndex(CT_PointCloudIndexVector* cloudIndex)
        {
            if (_cloudIndex != nullptr)
            {
                delete _cloudIndex;
            }
            _cloudIndex = cloudIndex;
        }

        void deletePointCloudIndex()
        {
            delete _cloudIndex;
            _cloudIndex = nullptr;
        }

        CT_AreaShape2DData* _area;
        CT_AbstractExporter* _exporter;
        CT_PointCloudIndexVector* _cloudIndex;
    };

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
