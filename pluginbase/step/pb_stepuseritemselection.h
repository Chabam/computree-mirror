#ifndef PB_STEPUSERITEMSELECTION_H
#define PB_STEPUSERITEMSELECTION_H

#include "ct_step/abstract/ct_abstractstep.h"

class CT_AbstractSingularItemDrawable;
class CT_StandardItemGroup;

/*!
 * \class PB_StepUserItemSelection
 * \ingroup Steps_PB
 * \brief <b>Etape qui permet de sélectionner manuellement des ItemDrawable et génère un résultat de sortie avec les items sélectionnés.</b>
 *
 * Detailed decription of step purpose.
 * Please also give a general view of the algorithm.
 *
 *
 *
 * <b>Input Models:</b>
 *
 * - CT_ResultGroup (Result)\n
 *     - CT_StandardItemGroup...\n
 *         - CT_AbstractItemDrawableWithPointCloud (Item)\n
 *
 * <b>Output Models:</b>
 *
 * - CT_ResultGroup (Result)\n
 *     - <em>cpy CT_StandardItemGroup...</em>\n
 *         - <em>cpy CT_Scene (Item)</em>\n
 *
 */

class PB_StepUserItemSelection: public CT_AbstractStep
{
    Q_OBJECT
    using SuperClass = CT_AbstractStep;

public:

    PB_StepUserItemSelection();

    QString description() const;

    CT_VirtualAbstractStep* createNewInstance() const final;

protected:

    void declareInputModels(CT_StepInModelStructureManager& manager) final;

    void fillPostInputConfigurationDialog(CT_StepConfigurableDialog* postInputConfigDialog) final;

    void declareOutputModels(CT_StepOutModelStructureManager& manager) final;

    void compute() final;

    void initManualMode();

    void useManualMode(bool quit = false);

private:

    // Step parameters
    QHash<CT_AbstractItemDrawable*, CT_StandardItemGroup*>      m_itemDrawableToAdd;
    QList<CT_AbstractItemDrawable*>                             m_itemDrawableSelected;
    DocumentInterface                                           *m_doc;
    int                                                         m_status;
    bool                                                        m_removeGroupsWithoutItemResearched;
    bool                                                        m_removeParents;

    int                                                         _mode;

    void recursiveRemoveGroup(CT_StandardItemGroup *parent, CT_StandardItemGroup *group) const;
};

#endif // PB_STEPUSERITEMSELECTION_H
