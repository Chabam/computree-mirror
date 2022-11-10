#ifndef PB_STEPUSERITEMSELECTION_H
#define PB_STEPUSERITEMSELECTION_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_itemdrawable/abstract/ct_abstractgeometricalitem.h"

class DocumentInterface;

class PB_StepUserItemSelection: public CT_AbstractStep
{
    Q_OBJECT
    using SuperClass = CT_AbstractStep;

public:

    PB_StepUserItemSelection();

    QString description() const final;
    QString detailledDescription() const final;
    QString inputDescription() const override;
    QString outputDescription() const override;

    CT_VirtualAbstractStep* createNewInstance() const final;

protected:

    void declareInputModels(CT_StepInModelStructureManager& manager) final;

    void declareOutputModels(CT_StepOutModelStructureManager& manager) final;

    void compute() final;

    void initManualMode() override;

    void useManualMode(bool quit = false) override;

private:

    // Step parameters
    QHash<CT_AbstractGeometricalItem*, CT_StandardItemGroup*>   m_itemDrawableToAdd;
    QList<CT_AbstractGeometricalItem*>                          m_itemDrawableSelected;
    DocumentInterface                                           *m_doc;
    int                                                         m_status;

    CT_HandleInResultGroupCopy<>                                mInResultCpy;
    CT_HandleInStdZeroOrMoreGroup                               mInRootGroup;
    CT_HandleInStdGroup<>                                       mInGroup;
    CT_HandleInSingularItem<CT_AbstractGeometricalItem>         mInItem;
};

#endif // PB_STEPUSERITEMSELECTION_H
