#ifndef PB_STEPEXPORTATTRIBUTESASVECTOR_H
#define PB_STEPEXPORTATTRIBUTESASVECTOR_H

#include <QMap>

#include "ct_step/abstract/ct_abstractstep.h"

#include "ct_itemdrawable/ct_plotlistingrid.h"
#include "ct_itemdrawable/ct_loopcounter.h"

#ifdef USE_GDAL
#include "gdal_priv.h"
#include "ogr_core.h"
#include "ogrsf_frmts.h"
#include "cpl_conv.h"

#include "readers/tools/gdal/ct_gdaltools.h"
#endif


class PB_StepExportAttributesAsVector: public CT_AbstractStep
{
    Q_OBJECT
    using SuperClass = CT_AbstractStep;

public:

    PB_StepExportAttributesAsVector();

    QString description() const final;
    QString detailledDescription() const final;
    QString inputDescription() const final;
    QString outputDescription() const final;
    QString detailsDescription() const final;

    CT_VirtualAbstractStep* createNewInstance() const final;

protected:

    void declareInputModels(CT_StepInModelStructureManager& manager) final;

    void fillPostInputConfigurationDialog(CT_StepConfigurableDialog* postInputConfigDialog) final;

    void declareOutputModels(CT_StepOutModelStructureManager& manager) final;

    void compute() final;

private:

#ifdef USE_GDAL
    struct GDalDatasetScopedPointerCustomDeleter
    {
        static inline void cleanup(GDALDataset* pointer)
        {
            GDALClose(pointer);
        }
    };

    QMap<QString, GDALDriver*> _gdalRasterDrivers;
    QMap<QString, GDALDriver*> _gdalVectorDrivers;
    QMap<QString, OGRFieldType>  _ogrTypes;
#endif


    CT_HandleInResultGroup<>                                        mInResult;
    CT_HandleInStdZeroOrMoreGroup                                   mInRootGroup;
    CT_HandleInStdGroup<>                                           mInGroupMain;
    CT_HandleInSingularItem<CT_PlotListInGrid>                      mInPlotListInGrid;
    CT_HandleInSingularItem<CT_AbstractSingularItemDrawable>        mInItemWithXY;
    CT_HandleInStdItemAttribute<CT_AbstractCategory::DOUBLE>        mInItemAttributeX;
    CT_HandleInStdItemAttribute<CT_AbstractCategory::DOUBLE>        mInItemAttributeY;

    CT_HandleInSingularItem<CT_AbstractSingularItemDrawable, 0, -1> mInItemWithAttribute; // optionnal and multiple
    CT_HandleInStdItemAttribute<CT_AbstractCategory::ANY, 0, -1>    mInItemAttribute; // optionnal and multiple

    CT_HandleInResultGroup<0,1>                                     mInResultCounter;
    CT_HandleInStdGroup<>                                           mInGroupCounter;
    CT_HandleInSingularItem<CT_LoopCounter>                         mInLoopCounter;

    QList<QString>          _modelsKeys;
    QList<QString>          _modelsKeysWithoutXOrYAttribute;

    QMap<QString, QString>  _names;
    QMap<QString, QString>  _shortNames;
    QMap<QString, QString>  _itemNames;

    // Step parameters
    bool        _replaceSpecialCharacters;
    bool        _shortenNames;

    QString     _vectorDriverName;
    QString     _vectorPrefix;
    QStringList _outVectorFolder;

    QString createExportBaseName(bool& first) const;
    void computeModelsKeysAndNamesAndOgrTypes();

    template<typename InHandleT>
    void computeModelsKeysAndNamesAndOgrTypes(InHandleT& handle, bool isNotXOrYAttribute)
    {
        auto it = handle.template iterateSelectedOutputModels<CT_OutAbstractItemAttributeModel>(mInResult);
        auto begin = it.begin();
        auto end = it.end();

        while(begin != end)
        {
            if(isStopped())
                return;

            const CT_OutAbstractItemAttributeModel* attModel = (*begin);
            const CT_OutAbstractModel* itemModel = static_cast<const CT_OutAbstractModel*>(attModel->parentModel());

            computeModelsKeysAndNamesAndOgrTypesForModels(itemModel, attModel, isNotXOrYAttribute);

            ++begin;
        }
    }

    void computeModelsKeysAndNamesAndOgrTypesForModels(const CT_OutAbstractModel* itemModel, const CT_OutAbstractItemAttributeModel* attModel, bool isNotXOrYAttribute);
    QString computeKeyForModels(const CT_OutAbstractModel* itemModel, const CT_OutAbstractModel* attModel) const;

#ifdef USE_GDAL
    void preExportVectorIfNecessary(const QString& exportBaseName, QScopedPointer<GDALDataset, GDalDatasetScopedPointerCustomDeleter>& vectorDataSet, OGRLayer*& vectorLayer);
#endif

    void addToIndexedAttributesCollection(const CT_AbstractSingularItemDrawable* item, const CT_AbstractItemAttribute* attribute, QMap<QString, QPair<const CT_AbstractSingularItemDrawable*, const CT_AbstractItemAttribute*> >& indexedAttributes) const;

    void replaceSpecialCharacters(QMap<QString, QString> &names) const;

};

#endif // PB_StepExportAttributesAsVector_H