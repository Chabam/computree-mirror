#include "pb_stepcomputepointmetrics.h"

#include "pb_steppluginmanager.h"

#include "ct_abstractstepplugin.h"
#include "ctliblas/metrics/abstract/ct_abstractmetric_las.h"

#include "ct_view/elements/ctg_configurableelementsselector.h"

#include "tools/ct_configurableelementtools.h"

PB_StepComputePointMetrics::PB_StepComputePointMetrics() : SuperClass()
{
}

PB_StepComputePointMetrics::~PB_StepComputePointMetrics()
{
    qDeleteAll(m_selectedXYZMetrics.begin(), m_selectedXYZMetrics.end());
    m_selectedXYZMetrics.clear();
}

// Step description (tooltip of contextual menu)
QString PB_StepComputePointMetrics::description() const
{
    return tr("Métriques de points (XYZ)");
}

QString PB_StepComputePointMetrics::detailledDescription() const
{
    return tr("Cette étape regroupe toutes les métriques de points disponibles dans les différents plugins actifs.<br><br>"
              "Dans Computree une \"métrique\" est un indicateur calculé sur un type de données précis. "
              "Les métriques de points sont calculées à partir d'un nuage de points. "
              "A minima les coordonnées (x,y,z) des points, et dans certains cas d'autres attributs issus du format standard LAS. "
              "De plus, une emprise peut optionnellement être fournie pour sélectionner les points à prendre en compte."
              "<br><br>"
              "Voici la liste des métriques de points disponibles :"
              "<br><br>"
              "%1").arg(CT_ConfigurableElementTools::formatHtmlStepDetailledDescription(pluginStaticCastT<PB_StepPluginManager>()->xyzMetricsAvailable()));
}

QString PB_StepComputePointMetrics::inputDescription() const
{
    return SuperClass::inputDescription() + tr("<br><br>Toutes les métriques de points prennent les mêmes données en entrée :<br>"
                                               "<ul>"
                                               "<li>Un nuage de points. C'est avec les coordonnées de ce nuage de points que les métriques sont calculées.</li>"
                                               "<li>Optionnellement une emprise. Si elle est sélectionnée, seuls les points inclus dans cette emprise sont pris en compte pour le calcul.</li>"
                                               "<li>Optionnellement des attributs LAS. Certaines métriques nécessitent des attributs complémentaires en plus des coordonnées. Si les attributs LAS ne sont pas sélectionnés, ces metriques seront fixées à leur valeur par défaut.</li>"
                                               "</ul>");
}

QString PB_StepComputePointMetrics::outputDescription() const
{
    return tr("Cette étape ajoute au résultat d'entrée un conteneur \"métriques\", contenant toutes les métriques calculées. ");
}

QString PB_StepComputePointMetrics::detailsDescription() const
{
    return tr("Il faut prendre garde à deux aspects lors de l'utilisation des métriques de points.<br><br>"
              "Premièrement, est-ce que la métrique nécessite les attributs LAS pour son calcul ? Si oui, il faut impérativement sélectionner ces attributs dans les données d'entrée, sous peine d'obtenir la valeur par défaut pour la métrique systématiquement.<br><br>"
              "Deuxièmement, les nuages de points sont originellement codés en altitude absolue. Mais il est fréquent de modifier ces nuages en soutrayant l'altitude du sol, afin d'obtenir des nuages de points en hauteur, où le relief est \"retiré\".<br>"
              "Il est donc important de savoir si un nuage des points est en Altitude (Alt) ou en Hauteur (Ht).<br>"
              "<ul>"
              "<li>Certaines métriques sont indifférentes, et fonctionnent dans les deux cas.</li>"
              "<li>D'autres n'ont de sens que pour un nuage en altitude</li>"
              "<li>D'autres n'ont de sens que pour un nuage en hauteur</li>"
              "</ul>"
              "C'est à l'utilisateur de vérifier les métriques adaptées au nuage de points fourni. <br>"
              "Pour éclairer ce choix, les titres de métriques contiennent généralement les mots clé Alt, Ht ou les deux. S'il n'y a pas de précision, la métrique fonctionne a priori pour les deux cas (en cas de doute se reporter à sa description ci-dessus)."
              "<br><br>"
              "<div class=\"container\">"
              "<details>"
              "<summary>"
              "What is the meaning of life ?"
              "</summary>"
              "<div>42</div>"
              "</details>"
              "</div><br><br>");
}


void PB_StepComputePointMetrics::savePostSettings(SettingsWriterInterface &writer) const
{
    CT_ConfigurableElementTools::saveSettingsOfACollectionOfConfigurableElement(m_selectedXYZMetrics,
                                                                                this,
                                                                                "Metric",
                                                                                writer);

    SuperClass::savePostSettings(writer);
}

bool PB_StepComputePointMetrics::restorePostSettings(SettingsReaderInterface &reader)
{
    if(!CT_ConfigurableElementTools::restoreSettingsOfConfigurableElementAndSaveItInACollection(m_selectedXYZMetrics,
                                                                                                pluginStaticCastT<PB_StepPluginManager>()->xyzMetricsAvailable(),
                                                                                                this,
                                                                                                "Metric",
                                                                                                reader))
        return false;

    return SuperClass::restorePostSettings(reader);
}

CT_VirtualAbstractStep* PB_StepComputePointMetrics::createNewInstance() const
{
    return new PB_StepComputePointMetrics();
}

void PB_StepComputePointMetrics::declareInputModels(CT_StepInModelStructureManager& manager)
{
    manager.addResult(mInResult, tr("Points"));
    manager.setZeroOrMoreRootGroup(mInResult, mInRootGroup);
    manager.addGroup(mInRootGroup, mInGroup, tr("Groupe"));
    manager.addItem(mInGroup, mInItem, tr("Scène"));
    manager.addItem(mInGroup, mInArea, tr("Emprise de la placette"));
    manager.addItem(mInGroup, mInLasPointsAttributesContainer, tr("Attributs LAS"));
}

bool PB_StepComputePointMetrics::postInputConfigure()
{
    CTG_ConfigurableElementsSelector cd(nullptr, hasChildrens());
    cd.setWindowTitle("Métriques séléctionnées");
    cd.setElementsAvailable(pluginStaticCastT<PB_StepPluginManager>()->xyzMetricsAvailable());
    cd.setElementsSelected(&m_selectedXYZMetrics);

    QObject::connect(&cd, SIGNAL(showHelp()), this, SLOT(openHelpPageForStep()));

    if(cd.exec() == QDialog::Accepted)
    {
        setSettingsModified(true);
        return true;
    }

    return false;
}

void PB_StepComputePointMetrics::finalizePostSettings()
{
    for(CT_AbstractConfigurableElement* f : m_selectedXYZMetrics)
    {
        static_cast<CT_AbstractMetric_XYZ*>(f)->finalizeConfiguration();
    }
}

void PB_StepComputePointMetrics::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    const CT_OutAbstractModel* outItemModel = mInItem.outModelSelected(mInResult);

    manager.addResultCopy(mInResult);
    manager.addItem(mInGroup, mOutAttributeList, tr("Métriques") + (outItemModel == nullptr ? QString() : tr(" (%1)").arg(outItemModel->displayableName())));

    for(CT_AbstractConfigurableElement* f : m_selectedXYZMetrics)
    {
        CT_AbstractMetric_XYZ* metric = static_cast<CT_AbstractMetric_XYZ*>(f);
        metric->declareOutputItemAttributeModelsInItem(manager, mOutAttributeList);
    }
}

void PB_StepComputePointMetrics::compute()
{
    for(CT_StandardItemGroup* group : mInGroup.iterateOutputs(mInResult))
    {
        const CT_AbstractItemDrawableWithPointCloud* points = group->singularItem(mInItem);

        if(points != nullptr)
        {
            const CT_AbstractAreaShape2D* plotArea = group->singularItem(mInArea);

            const CT_AreaShape2DData* area = nullptr;

            if(plotArea != nullptr)
                area = &plotArea->getAreaData();

            const CT_StdLASPointsAttributesContainer* lasAtt = group->singularItem(mInLasPointsAttributesContainer);

            CT_ItemAttributeList* outAttributes = new CT_ItemAttributeList();
            group->addSingularItem(mOutAttributeList, outAttributes);

            for(CT_AbstractConfigurableElement* f : m_selectedXYZMetrics)
            {
                CT_AbstractMetric_XYZ* metric = static_cast<CT_AbstractMetric_XYZ*>(f);
                CT_AbstractMetric_LAS* lasMetric = dynamic_cast<CT_AbstractMetric_LAS*>(metric);

                bool ok = false;

                if(lasMetric != nullptr)
                    ok = lasMetric->initLasDatas(points->pointCloudIndex(), area, lasAtt);
                else
                    ok = metric->initDatas(points->pointCloudIndex(), area);

                if (ok)
                    metric->computeMetric(outAttributes);
            }
        }
    }
}
