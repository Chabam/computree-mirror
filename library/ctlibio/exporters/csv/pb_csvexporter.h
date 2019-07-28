#ifndef PB_CSVEXPORTER_H
#define PB_CSVEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class PB_CSVExporterConfiguration;
class CT_OutAbstractSingularItemModel;
class CT_OutAbstractItemAttributeModel;

class PB_CSVExporter : public CT_AbstractExporter
{
    Q_OBJECT
    typedef CT_AbstractExporter SuperClass;

public:
    PB_CSVExporter();
    PB_CSVExporter(const PB_CSVExporter& other);
    ~PB_CSVExporter();

    QString getExporterCustomName() const override;

    CT_StepsMenu::LevelPredefined getExporterSubMenuName() const override;

    void init() override;

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list) override;

    bool configureExport() override;

    void saveSettings(SettingsWriterInterface& writer) const override;
    bool restoreSettings(SettingsReaderInterface& reader) override;

    CT_EXPORTER_DECL_COPY(PB_CSVExporter)

protected:

    bool protectedExportToFile() override;

private:

    PB_CSVExporterConfiguration                                                 *_configuration;
    QMap<CT_OutAbstractSingularItemModel*, QList<CT_AbstractItemDrawable*>* >   _mapItemToExport;
    bool                                                                        _mapKeyChanged;

    bool configureExportWithLastConfigurationAndNewItemToExport();

    void clearMap();
    CT_OutAbstractSingularItemModel* getItemModelByName(const QString &name) const;
    CT_OutAbstractItemAttributeModel* getItemAttributeModelByName(const CT_OutAbstractSingularItemModel *sItem, const QString &name) const;
};

#endif // PB_CSVEXPORTER_H
