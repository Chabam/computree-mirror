#ifndef CT_INRESULTMODELGROUPTOCOPY_H
#define CT_INRESULTMODELGROUPTOCOPY_H

#include "ct_result/model/inModel/ct_inresultmodelgroup.h"

class CT_OutResultModelGroupToCopyPossibility;
class CT_OutAbstractResultModel;

/**
 * @brief A INPUT model that represent a CT_ResultGroup that you want to search and COPY ! Per example if you want to
 *        add other information like Item or Attributes, etc...
 */
class PLUGINSHAREDSHARED_EXPORT CT_InResultModelGroupToCopy : public CT_InResultModelGroup
{
    Q_OBJECT
    typedef CT_InResultModelGroup SuperClass;

public:

    /**
     * @brief Constructor
     * @param uniqueName : the unique name (in the hierarchy) of the model. Typically a DEF_...
     * @param rootGroupModel : the root model that represent a group.
     * @param displayableName : a displayable name for this model. If empty by default used the unique name. Empty by default.
     * @param description : a description of the result you search. If empty by default used the displayable name. Empty by default.
     * @param recursive : true if you want this model research recusively results in all step in the tree (go up in hierarchy) or false
     *                    if you want just research in results of the step passed in parameter of the method "searchResultsInStep...". False by default.
     */
    CT_InResultModelGroupToCopy(const QString &uniqueName,
                                CT_InAbstractGroupModel *rootGroupModel,
                                const QString &displayableName = "",
                                const QString &description = "",
                                bool recursive = false);

    /*!
     *  \brief Retourne une liste des modèles de sortie que vous
     *         pouvez modifier si vous le souhaitez.
     *
     *  ATTENTION : Cette méthode crée de nouveaux objets. Vous êtes responsable
     *              de leur vie en mémoire ! Donc n'oubliez pas de les supprimer si vous
     *              ne vous en servez pas !!!!!
     */
    QList<CT_OutResultModelGroupToCopyPossibility*> getOutResultModelGroupsSelectedToCopy() const;

    /*!
     *  \brief Retourne une liste des résultats modèles de sortie que vous devez utiliser pour
     *         rechercher vos modèles d'entrée avec "getInModel(...)". Exemple :
     *
     *         CT_InResultModelGroupToCopy *intResModel = (CT_InResultModelGroupToCopy*)getInResultModel(DEF_SearchInResult);
     *
     *         const CT_OutAbstractResultModel *outModelForSearchInModel = intResModel->getOutResultModelForSearchInModel().first();
     *
     *         CT_InAbstractSingularItemModel *inPointClusterModel = (CT_InAbstractSingularItemModel*)getInModel(*outModelForSearchInModel, DEF_SearchInPointCloud);
     */
    QList<const CT_OutAbstractResultModel*> getOutResultModelForSearchInModel() const;

    /**
     * @brief Save all necessary elements to be able to restore it with the method "restoreSettings"
     */
    void saveSettings(SettingsWriterInterface& writer) const override;

    /**
     * @brief Restore the state
     */
    bool restoreSettings(SettingsReaderInterface& reader) override;

    // INHERITED FROM CT_InAbstractModel //
    CT_InAbstractModel* copy(bool withPossibilities) const override;
};

#endif // CT_INRESULTMODELGROUPTOCOPY_H
