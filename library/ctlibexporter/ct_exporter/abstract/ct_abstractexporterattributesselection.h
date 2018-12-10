#ifndef CT_ABSTRACTEXPORTERATTRIBUTESSELECTION_H
#define CT_ABSTRACTEXPORTERATTRIBUTESSELECTION_H

#include "ct_exporter/abstract/ct_abstractexporter.h"
#include "ct_view/itemdrawable/ct_itemdrawablehierarchycollectionmodel.h"
#include "ct_tools/itemdrawable/abstract/ct_abstractitemdrawablecollectionbuilder.h"
#include "ct_view/itemdrawable/ct_itemdrawablehierarchycollectionwidget.h"

/**
 * @brief Exporter that have predefined method to search other ItemDrawable or Attributes like CT_AbstractPointsAttributes, CT_AbstractFaceAttributes, etc...
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractExporterAttributesSelection : public CT_AbstractExporter
{
    Q_OBJECT
    typedef CT_AbstractExporter SuperClass;

public:
    CT_AbstractExporterAttributesSelection();
    CT_AbstractExporterAttributesSelection(const CT_AbstractExporterAttributesSelection& other);
    ~CT_AbstractExporterAttributesSelection();

    /**
     * @brief Set to true to search only models in step and not real item drawable (false by default)
     */
    void setSearchOnlyModels(bool enable);

    /**
     * @brief Returns true if we must only search models
     */
    bool searchOnlyModels() const;

    /**
     * @brief By default call selectAttributes()
     */
    bool configureExport() override;

    /**
     * @brief Call this method to show the dialog where the user can select attributes
     */
    bool selectAttributes();

    /**
     * @brief Return true if this exporter can export colors information from the document or from attributes
     */
    bool canExportColors() const;

    /**
     * @brief Return true if this exporter can export normals information from the document or from attributes
     */
    bool canExportNormals() const;

protected:

    /**
     * @brief Create builders that must search attributes or ItemDrawable. Per example a CT_ItemDrawableCollectionBuilderT<CT_AbstractPointsAttributes>
     *        and a CT_ItemDrawableCollectionBuilderT<CT_Scanner>
     *
     *        The first element of the QPair is the name that you want to show in the tree view for the results of the builder and the second element the builder
     */
    virtual QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > createBuilders() const = 0;

    /**
     * @brief Use elements selected. Return false if you want to cancel the configuration.
     */
    virtual bool useSelection(const CT_ItemDrawableHierarchyCollectionWidget *selectorWidget) = 0;

    /**
     * @brief Call this method in your constructor to set if we can display document's colors and color attributes in the selector (dialog). false by default.
     */
    void setCanExportColors(bool enable);

    /**
     * @brief Call this method in your constructor to set if we can display document's normals and normal attributes in the selector (dialog). false by default.
     */
    void setCanExportNormals(bool enable);

private:
    QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> >  m_buildersResults;
    bool                                                                m_canExportColorsOfDocument;
    bool                                                                m_canExportNormalsOfDocument;
    bool                                                                m_searchModels;

    /**
     * @brief Remove builders from the list and from the memory
     */
    void clearBuilders();

    /**
     * @brief Return the step where the research for attributes must begin
     */
    CT_VirtualAbstractStep* rootStepWhereToBeginSearch() const;

    /**
     * @brief Build the collection of attributes for the selection widget
     */
    void buildItemsAndAttributesCollection();

    /**
     * @brief Return the model to pass to the CT_ItemDrawableHierarchyCollectionWidget
     */
    CT_ItemDrawableHierarchyCollectionModel* convertResultOfBuildersToModelForSelectorWidget() const;
};

#endif // CT_ABSTRACTEXPORTERATTRIBUTESSELECTION_H
