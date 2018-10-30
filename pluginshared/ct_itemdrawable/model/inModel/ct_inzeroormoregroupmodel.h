#ifndef CT_INZEROORMOREGROUPMODEL_H
#define CT_INZEROORMOREGROUPMODEL_H

#include "ct_itemdrawable/model/inModel/abstract/ct_inabstractgroupmodel.h"

/*!
 * Use this class to get the possibility to search children anywhere in the tree.
 *
 * Typically with use this class if you want to search a group with childrens anywhere in a tree :
 *
 * INPUT example :
 *
 * IR (CT_InResultModelGroup)
 * |--- IG (CT_InZeroOrMoreGroupModel)
 *      |--- IG (CT_InStdGroupModel / Obligatory)
 *           |--- II (CT_InStdSingularItem => CT_Circle::staticGetType() / Obligatory / OneIfMultiple)
 *
 * OUTPUT example 1 :
 *
 * OR (Result = "R")
 * |--- OG (Group = "G")
 *      |--- OI (CT_Circle = "C")
 *      |--- OI (CT_PointCluster = "P")
 *
 * OUTPUT example 2:
 *
 * OR (Result = "R")
 * |--- OG (Group = "G1")
 *      |--- OG (Group = "G2")
 *           |--- OI (CT_Circle = "C")
 *
 * OUTPUT example 3:
 *
 * OR (Result = "R")
 * |--- OG (Group = "G1")
 *      |--- OG (Group = "G2")
 *      |    |--- OI (CT_Circle = "C1")
 *      |--- OI (CT_Circle = "C2")
 *
 * For the first output example the CT_InStdGroupModel will find "G"
 * For the second output example the CT_InStdGroupModel will find "G2"
 * For the second output example the CT_InStdGroupModel will find "G1" and "G2" but the user must choose one because the circle is set with choiceMode() == OneIfMultiple
 */
class PLUGINSHAREDSHARED_EXPORT CT_InZeroOrMoreGroupModel : public CT_InAbstractGroupModel
{
    Q_OBJECT

public:

    /*!
     *  \brief Constructeur
     *
     *  Constructeur de la classe. INFORMATION : Ce modèle ne peut pas être récupéré puisqu'il ne renvoie rien comme nom de modèle (uniqueName())
     */
    CT_InZeroOrMoreGroupModel();

    /**
     * @brief Verify if this model (only obligatory) have at least one possibility selected.
     * @return true is the test pass.
     */
    bool isAtLeastOnePossibilitySelectedIfItDoes(QStringList *errors = NULL) const override;

    // OVERLOADED FROM CT_AbstractItemModel //
    bool canSetUniqueName(const QString& uniqueName) const override { Q_UNUSED(uniqueName) return false; }

    /**
     * @brief Copy this model
     */
    CT_InAbstractModel* copy(bool withPossibilities) const override;

    // INHERITED FROM CT_AbstractGroupModelT //
    DEF_CT_AbstractGroupModelIn* copyGroup() const override { return (DEF_CT_AbstractGroupModelIn*)copy(true); }

    /**
     * @brief Return true if this model can be compared with the OUTPUT model passed in parameter
     */
    bool canBeComparedWith(const CT_OutAbstractModel &model) const override;

protected:
    /**
     * @brief Must return true if the model passed in parameter can be added in possibilities
     */
    bool canAddPossibilityForThisModel(const CT_OutAbstractModel &model) const override;

    /**
     * @brief Must return true if the current OUTPUT model can be compared with children of the OUTPUT model.
     */
    bool mustCompareThisWithChildrenOfOutModel() const override;

    /**
     * @brief Must return true if the current OUTPUT model must be compared with children of this INPUT model.
     */
    bool mustCompareOutModelWithChildrenOfThisModel() const override;


    // OVERLOADED FROM CT_AbstractGroupModelT //
    void groupAdded(const DEF_CT_AbstractGroupModelIn *group) override {  Q_UNUSED(group)  internalSetFinderMode(CT_InAbstractModel::F_IsObligatory);    }
    void itemAdded(const CT_InAbstractSingularItemModel *item) override { Q_UNUSED(item)   internalSetFinderMode(CT_InAbstractModel::F_IsObligatory);    }
    void groupRemoved(const DEF_CT_AbstractGroupModelIn *group) override { Q_UNUSED(group)     internalSetFinderMode(isEmpty() ? CT_InAbstractModel::F_IsOptional : CT_InAbstractModel::F_IsObligatory);    }
    void itemRemoved(const CT_InAbstractSingularItemModel *item) override { Q_UNUSED(item)     internalSetFinderMode(isEmpty() ? CT_InAbstractModel::F_IsOptional : CT_InAbstractModel::F_IsObligatory);    }
};

#endif // CT_INZEROORMOREGROUPMODEL_H
