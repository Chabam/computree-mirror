#ifndef CT_ABSTRACTEXPORTERPOINTATTRIBUTESSELECTION_H
#define CT_ABSTRACTEXPORTERPOINTATTRIBUTESSELECTION_H

#include "ct_exporter/abstract/ct_abstractexporterattributesselection.h"
#include "ct_tools/attributes/ct_attributestocloudworkert.h"
#include "ct_tools/itemdrawable/ct_itemdrawablecollectionbuildert.h"
#include "ct_itemdrawable/ct_pointsattributescolor.h"
#include "ct_itemdrawable/ct_pointsattributesnormal.h"
#include "ct_itemdrawable/abstract/ct_abstractpointattributesscalar.h"

/**
 * @brief Inherits from this class if you want that your exporter can use attributes for points (like colors, etc...)
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractExporterPointAttributesSelection : public CT_AbstractExporterAttributesSelection
{
    Q_OBJECT
    typedef CT_AbstractExporterAttributesSelection SuperClass;

public:
    CT_AbstractExporterPointAttributesSelection();
    CT_AbstractExporterPointAttributesSelection(const CT_AbstractExporterPointAttributesSelection& other) : SuperClass(other) {}

    bool configureExport();

    QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > createBuilders() const override;
    bool useSelection(const CT_ItemDrawableHierarchyCollectionWidget *selectorWidget);

    void saveSettings(SettingsWriterInterface& writer) const override;
    bool restoreSettings(SettingsReaderInterface& reader) override;

protected:
    typedef CT_ItemDrawableCollectionBuilderT<CT_PointsAttributesColor, CT_AbstractPointAttributesScalar>   PointColorsCollectionBuilder;
    typedef CT_ItemDrawableCollectionBuilderT<CT_PointsAttributesNormal>                                    PointNormalsCollectionBuilder;


    CT_AttributesToCloudWorkerT<CT_AbstractPointsAttributes>                                        m_attributsColorPointWorker;
    CT_AttributesToCloudWorkerT<CT_AbstractPointsAttributes>                                        m_attributsNormalPointWorker;

    QList<CT_OutAbstractSingularItemModel*>                                                         m_attributesColorModel;
    QList<CT_OutAbstractSingularItemModel*>                                                         m_attributesNormalModel;

    /**
     * @brief You must call this method to create the color cloud. Call this method in the beginning of your method "protectedExportToFile()". It will
     *        return the color cloud that you must use (WARNING : the color cloud returned can be NULL ! if the user don't select a point attributes)
     */
    CT_AbstractColorCloud* createColorCloudBeforeExportToFile();

    /**
     * @brief You must call this method to create the normal cloud. Call this method in the beginning of your method "protectedExportToFile()". It will
     *        return the normal cloud that you must use (WARNING : the normal cloud returned can be NULL ! if the user don't select a point attributes)
     */
    CT_AbstractNormalCloud* createNormalCloudBeforeExportToFile();

    /**
     * @brief You must call this method before you quit the method "protectedExportToFile()"
     */
    virtual void clearWorker();
};

#endif // CT_ABSTRACTEXPORTERPOINTATTRIBUTESSELECTION_H
