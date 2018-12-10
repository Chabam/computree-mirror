#include "ct_abstractexporterpointattributesselection.h"

#include "ct_itemdrawable/tools/ct_itemsearchhelper.h"

#include "ct_model/tools/ct_modelsaverestorehelper.h"

CT_AbstractExporterPointAttributesSelection::CT_AbstractExporterPointAttributesSelection() : CT_AbstractExporterAttributesSelection()
{
    setCanExportPoints(true);
}

bool CT_AbstractExporterPointAttributesSelection::configureExport()
{
    clearWorker();

    return selectAttributes();
}

QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > CT_AbstractExporterPointAttributesSelection::createBuilders() const
{
    QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > l;

    if(canExportColors())
        l << QPair<QString, CT_AbstractItemDrawableCollectionBuilder*>(QObject::tr("Colors"), new PointColorsCollectionBuilder(false, true));

    if(canExportNormals())
        l << QPair<QString, CT_AbstractItemDrawableCollectionBuilder*>(QObject::tr("Normals"), new PointNormalsCollectionBuilder(false, false, true));

    return l;
}

bool CT_AbstractExporterPointAttributesSelection::useSelection(const CT_ItemDrawableHierarchyCollectionWidget *selectorWidget)
{
    clearWorker();

    // TODO : verify that this two lines is correct because i don't understant what we use clouds
    //        of the document ? And what happen if no cloud is selected ?
    m_attributsColorPointWorker.setColorCloud(selectorWidget->colorCloudOfDocumentSelected());
    m_attributsNormalPointWorker.setNormalCloud(selectorWidget->normalCloudOfDocumentSelected());

    if(searchOnlyModels()) {

        QList<CT_OutAbstractSingularItemModel*> list = selectorWidget->itemDrawableModelSelected();
        QListIterator<CT_OutAbstractSingularItemModel*> it(list);

        while(it.hasNext())
        {
            CT_OutAbstractSingularItemModel *model = it.next();
            CT_AbstractItemDrawable *item = model->itemDrawable();
            CT_PointsAttributesColor *pac = dynamic_cast<CT_PointsAttributesColor*>(item);

            if(pac != NULL) {
                m_attributesColorModel.append(model);
            } else {
                CT_AbstractPointAttributesScalar *pas = dynamic_cast<CT_AbstractPointAttributesScalar*>(item);

                if(pas != NULL) {
                    m_attributesColorModel.append(model);
                } else {

                    CT_PointsAttributesNormal *pan = dynamic_cast<CT_PointsAttributesNormal*>(item);

                    if(pan != NULL)
                        m_attributesNormalModel.append(model);
                }
            }
        }

    } else {

        QList<CT_AbstractPointsAttributes*> attributesColor;
        QList<CT_AbstractPointsAttributes*> attributesNormal;

        QList<CT_AbstractSingularItemDrawable*> list = selectorWidget->itemDrawableSelected();
        QListIterator<CT_AbstractSingularItemDrawable*> it(list);

        while(it.hasNext())
        {
            CT_AbstractSingularItemDrawable *item = it.next();
            CT_PointsAttributesColor *pac = dynamic_cast<CT_PointsAttributesColor*>(item);

            if(pac != NULL) {
                attributesColor.append(pac);
            } else {
                CT_AbstractPointAttributesScalar *pas = dynamic_cast<CT_AbstractPointAttributesScalar*>(item);

                if(pas != NULL) {
                    attributesColor.append(pas);
                } else {

                    CT_PointsAttributesNormal *pan = dynamic_cast<CT_PointsAttributesNormal*>(item);

                    if(pan != NULL)
                        attributesNormal.append(pan);
                }
            }
        }

        m_attributsColorPointWorker.setAttributes(attributesColor);
        m_attributsNormalPointWorker.setAttributes(attributesNormal);
    }

    return true;
}

void CT_AbstractExporterPointAttributesSelection::saveSettings(SettingsWriterInterface &writer) const
{
    SuperClass::saveSettings(writer);

    for(const CT_OutAbstractSingularItemModel* model : m_attributesColorModel) {
        CT_ModelSaveRestoreHelper::saveSettingsOfOutModel(this, writer, model, "ColorModel");
    }

    for(const CT_OutAbstractSingularItemModel* model : m_attributesNormalModel) {
        CT_ModelSaveRestoreHelper::saveSettingsOfOutModel(this, writer, model, "NormalModel");
    }
}

bool CT_AbstractExporterPointAttributesSelection::restoreSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restoreSettings(reader))
        return false;

    clearWorker();

    const int nColorModel = reader.parameterCount(this, "ColorModel");

    for(int i=0; i<nColorModel; ++i) {

        bool savedAndFoundedOrNotSaved;
        CT_OutAbstractSingularItemModel* model = dynamic_cast<CT_OutAbstractSingularItemModel*>(CT_ModelSaveRestoreHelper::restoreOutModelFromSettings(this, reader, myStep(), "ColorModel", savedAndFoundedOrNotSaved));

        if(!savedAndFoundedOrNotSaved)
            return false;

        if(model != NULL)
            m_attributesColorModel.append(model);
    }

    const int nNormalModel = reader.parameterCount(this, "NormalModel");

    for(int i=0; i<nNormalModel; ++i) {

        bool savedAndFoundedOrNotSaved;
        CT_OutAbstractSingularItemModel* model = dynamic_cast<CT_OutAbstractSingularItemModel*>(CT_ModelSaveRestoreHelper::restoreOutModelFromSettings(this, reader, myStep(), "NormalModel", savedAndFoundedOrNotSaved));

        if(!savedAndFoundedOrNotSaved)
            return false;

        if(model != NULL)
            m_attributesNormalModel.append(model);
    }

    return true;
}

CT_AbstractColorCloud* CT_AbstractExporterPointAttributesSelection::createColorCloudBeforeExportToFile()
{
    CT_AbstractColorCloud *cc = NULL;

    // if we have saved models for attributes of type colors we must search the real item drawable that match with this model
    if(!m_attributesColorModel.isEmpty()) {
        QList<CT_AbstractPointsAttributes*> attributesColor;

        CT_ItemSearchHelper helper;

        QListIterator<CT_OutAbstractSingularItemModel*> itM(m_attributesColorModel);

        while(itM.hasNext()) {
            CT_ResultItemIterator it = helper.searchSingularItemsForModel(itM.next());

            while(it.hasNext()) {
                CT_AbstractSingularItemDrawable *item = (CT_AbstractSingularItemDrawable*)it.next();

                if((dynamic_cast<CT_PointsAttributesColor*>(item) != NULL)
                        || (dynamic_cast<CT_AbstractPointAttributesScalar*>(item)))
                    attributesColor.append(dynamic_cast<CT_AbstractPointsAttributes*>(item));
            }
        }

        m_attributsColorPointWorker.setAttributes(attributesColor);
    }

    if((m_attributsColorPointWorker.colorCloud().data() == NULL)
            && (!m_attributsColorPointWorker.attributes().isEmpty()))
    {
        m_attributsColorPointWorker.setColorCloud(PS_REPOSITORY->createNewColorCloud(CT_Repository::SyncWithPointCloud));
        m_attributsColorPointWorker.apply();
        cc = m_attributsColorPointWorker.colorCloud()->abstractColorCloud();
    }
    else if(m_attributsColorPointWorker.colorCloud().data() != NULL)
    {
        // TODO MK 19.10.18 : verify if apply() must not be called first ???!!
        cc = m_attributsColorPointWorker.colorCloud()->abstractColorCloud();
    }

    return cc;
}

CT_AbstractNormalCloud* CT_AbstractExporterPointAttributesSelection::createNormalCloudBeforeExportToFile()
{
    CT_AbstractNormalCloud *nc = NULL;

    // if we have saved models for attributes of type colors we must search the real item drawable that match with this model
    if(!m_attributesNormalModel.isEmpty()) {
        QList<CT_AbstractPointsAttributes*> attributesNormal;

        CT_ItemSearchHelper helper;

        QListIterator<CT_OutAbstractSingularItemModel*> itM(m_attributesNormalModel);

        while(itM.hasNext()) {
            CT_ResultItemIterator it = helper.searchSingularItemsForModel(itM.next());

            while(it.hasNext()) {
                CT_AbstractSingularItemDrawable *item = (CT_AbstractSingularItemDrawable*)it.next();

                if(dynamic_cast<CT_PointsAttributesNormal*>(item) != NULL)
                    attributesNormal.append(dynamic_cast<CT_AbstractPointsAttributes*>(item));
            }
        }

        m_attributsNormalPointWorker.setAttributes(attributesNormal);
    }

    if((m_attributsNormalPointWorker.normalCloud().data() == NULL)
            && (!m_attributsNormalPointWorker.attributes().isEmpty()))
    {
        m_attributsNormalPointWorker.setNormalCloud(PS_REPOSITORY->createNewNormalCloud(CT_Repository::SyncWithPointCloud));
        m_attributsNormalPointWorker.apply();
        nc = m_attributsNormalPointWorker.normalCloud()->abstractNormalCloud();
    }
    else if(m_attributsNormalPointWorker.normalCloud().data() != NULL)
    {
        // TODO MK 19.10.18 : verify if apply() must not be called first ???!!
        nc = m_attributsNormalPointWorker.normalCloud()->abstractNormalCloud();
    }

    return nc;
}

void CT_AbstractExporterPointAttributesSelection::clearWorker()
{
    m_attributsColorPointWorker.setAttributes(QList<CT_AbstractPointsAttributes*>());
    m_attributsColorPointWorker.setColorCloud(QSharedPointer<CT_StandardColorCloudRegistered>(NULL));

    m_attributsNormalPointWorker.setAttributes(QList<CT_AbstractPointsAttributes*>());
    m_attributsNormalPointWorker.setColorCloud(QSharedPointer<CT_StandardColorCloudRegistered>(NULL));

    m_attributesColorModel.clear();
    m_attributesNormalModel.clear();
}
