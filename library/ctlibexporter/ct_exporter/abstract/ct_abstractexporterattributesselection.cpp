#include "ct_abstractexporterattributesselection.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"
#include "ct_result/abstract/ct_abstractresult.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QMessageBox>

CT_AbstractExporterAttributesSelection::CT_AbstractExporterAttributesSelection() : SuperClass()
{
    setCanExportColors(false);
    setCanExportNormals(false);
    setSearchOnlyModels(false);
}

CT_AbstractExporterAttributesSelection::CT_AbstractExporterAttributesSelection(const CT_AbstractExporterAttributesSelection& other) : SuperClass(other)
{
    for(const QPair<QString, CT_AbstractItemDrawableCollectionBuilder*>& pair : other.m_buildersResults) {
        m_buildersResults.append(qMakePair(pair.first, pair.second->copy()));
    }

    m_canExportColorsOfDocument = other.m_canExportColorsOfDocument;
    m_canExportNormalsOfDocument = other.m_canExportNormalsOfDocument;
    m_searchModels = other.m_searchModels;
}

CT_AbstractExporterAttributesSelection::~CT_AbstractExporterAttributesSelection()
{
    clearBuilders();
}

void CT_AbstractExporterAttributesSelection::setSearchOnlyModels(bool enable)
{
    m_searchModels = enable;
}

bool CT_AbstractExporterAttributesSelection::searchOnlyModels() const
{
    return m_searchModels;
}

bool CT_AbstractExporterAttributesSelection::configureExport()
{
    return selectAttributes();
}

bool CT_AbstractExporterAttributesSelection::selectAttributes()
{
    clearBuilders();

    if(!errorMessage().isEmpty())
    {
        QMessageBox::critical(NULL, QObject::tr("Erreur"), errorMessage(), QMessageBox::Ok);
        return false;
    }

    QDialog dialog;
    dialog.setWindowTitle(QObject::tr("Configuration des autres éléments de l'export"));
    dialog.resize(700,500);

    auto cloudType = [this]() -> CT_ItemDrawableHierarchyCollectionWidget::CloudType {
                        if(this->canExportPoints())
                            return CT_ItemDrawableHierarchyCollectionWidget::POINTS;
                        else if(this->canExportEdges())
                            return CT_ItemDrawableHierarchyCollectionWidget::EDGES;
                        else if(this->canExportFaces())
                            return CT_ItemDrawableHierarchyCollectionWidget::FACES;

                        return CT_ItemDrawableHierarchyCollectionWidget::NONE;
                    };

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                                     Qt::Vertical);
    CT_ItemDrawableHierarchyCollectionWidget *selectorWidget = new CT_ItemDrawableHierarchyCollectionWidget(cloudType(), &dialog);
    selectorWidget->setCanSelectColorsOfDocuments(m_canExportColorsOfDocument);
    selectorWidget->setCanSelectNormalsOfDocuments(m_canExportNormalsOfDocument);
    selectorWidget->setDocumentManager(documentManager());

    QHBoxLayout *layout = new QHBoxLayout(&dialog);
    layout->addWidget(selectorWidget);
    layout->addWidget(buttons);

    connect(buttons, SIGNAL(accepted()), &dialog, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), &dialog, SLOT(reject()));

    buildItemsAndAttributesCollection();

    selectorWidget->setModel(convertResultOfBuildersToModelForSelectorWidget());

    if(selectorWidget->hasChoice() &&
            (!selectorWidget->canChoiceBeSelectedByDefault() || !selectorWidget->selectChoiceByDefault()))
    {
        if(dialog.exec() == QDialog::Rejected)
            return false;
    }

    return useSelection(selectorWidget);
}

bool CT_AbstractExporterAttributesSelection::canExportColors() const
{
    return m_canExportColorsOfDocument;
}

bool CT_AbstractExporterAttributesSelection::canExportNormals() const
{
    return m_canExportNormalsOfDocument;
}

void CT_AbstractExporterAttributesSelection::setCanExportColors(bool enable)
{
    m_canExportColorsOfDocument = enable;
}

void CT_AbstractExporterAttributesSelection::setCanExportNormals(bool enable)
{
    m_canExportNormalsOfDocument = enable;
}

void CT_AbstractExporterAttributesSelection::clearBuilders()
{
    for(const QPair<QString, CT_AbstractItemDrawableCollectionBuilder*>& pair : m_buildersResults) {
        delete pair.second;
    }

    m_buildersResults.clear();
}

CT_VirtualAbstractStep* CT_AbstractExporterAttributesSelection::rootStepWhereToBeginSearch() const
{
    CT_VirtualAbstractStep* parent = myStep();

    if(parent == NULL)
    {
        QListIterator<CT_AbstractItemDrawable*> it(itemDrawableToExport());

        while(it.hasNext()
              && (parent == NULL))
        {
            CT_AbstractItemDrawable *id = it.next();

            if(id->result() != NULL)
                parent = id->result()->parentStep();
        }

        if(parent == NULL)
        {
            DocumentInterface *doc = myDocument();

            if(doc != NULL)
            {
                it = doc->getItemDrawable();

                while(it.hasNext()
                      && (parent == NULL))
                {
                    CT_AbstractItemDrawable *id = it.next();

                    if(id->result() != NULL)
                        parent = id->result()->parentStep();
                }
            }
        }
    }

    return ((parent != NULL) ? parent->rootStep() : NULL);
}

void CT_AbstractExporterAttributesSelection::buildItemsAndAttributesCollection()
{
    clearBuilders();

    CT_VirtualAbstractStep *parent = rootStepWhereToBeginSearch();

    if(parent != NULL)
    {
        const QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > builders = createBuilders();

        for(const QPair<QString, CT_AbstractItemDrawableCollectionBuilder*>& pair : builders)
        {
            CT_AbstractItemDrawableCollectionBuilder* builder = pair.second;

            if(builder->buildFrom(dynamic_cast<CT_VirtualAbstractStep*>(parent), m_searchModels))
                m_buildersResults.append(pair);
            else
                delete builder;
        }
    }
}

CT_ItemDrawableHierarchyCollectionModel* CT_AbstractExporterAttributesSelection::convertResultOfBuildersToModelForSelectorWidget() const
{
    CT_ItemDrawableHierarchyCollectionModel *model = new CT_ItemDrawableHierarchyCollectionModel();

    CT_ItemDrawableHierarchyCollectionSelectionModel *sModel;
    int index = 0;

    for(const QPair<QString, CT_AbstractItemDrawableCollectionBuilder*>& pair : m_buildersResults)
    {
        const QList<CT_ItemDrawableCollectionHierarchyStep>& collection = pair.second->collection();

        if(!collection.isEmpty()) {
            sModel = new CT_ItemDrawableHierarchyCollectionSelectionModel(pair.first);
            sModel->setCollection(pair.second->collection());

            if(pair.second->isMutuallyExclusive())
                sModel->addExcludeModel(index);

            if(pair.second->mustExcludeColorsOfDocuments())
                sModel->addExcludeModel(-1);

            if(pair.second->mustExcludeNormalsOfDocuments())
                sModel->addExcludeModel(-2);

            model->addModel(sModel);
            ++index;
        }
    }

    if(model->models().isEmpty())
    {
        delete model;
        model = NULL;
    }
    else
    {
        int currentIndex = 0;

        // add excluded model to other models
        QListIterator<CT_ItemDrawableHierarchyCollectionSelectionModel*> itM(model->models());

        while(itM.hasNext())
        {
            CT_ItemDrawableHierarchyCollectionSelectionModel *sModel = itM.next();

            // excluded modes of this selection model
            QList<int> excluded = sModel->excludedModels();

            foreach (int index, excluded)
            {
                if((index != currentIndex) && (index >= 0))
                {
                    // model excluded from this selection model
                    CT_ItemDrawableHierarchyCollectionSelectionModel *sModelExcluded = model->models().at(index);

                    // if this model has not already the current selection model in excluded list
                    if(!sModelExcluded->mustExcludeModel(currentIndex))
                        sModelExcluded->addExcludeModel(currentIndex);  // we add it
                }
            }

            ++currentIndex;
        }
    }

    return model;
}
