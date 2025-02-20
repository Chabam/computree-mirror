#ifndef PB_STEPUSEREADERTOLOADFILES_H
#define PB_STEPUSEREADERTOLOADFILES_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_itemdrawable/ct_fileheader.h"

/**
 * @brief Step that use a result that contains a collection of CT_FileHeader (in a group) and use
 *        it to load multiple files.
 */
class PB_StepUseReaderToLoadFiles : public CT_AbstractStep
{
    Q_OBJECT
    using SuperClass = CT_AbstractStep;

public:
    PB_StepUseReaderToLoadFiles();

    QString description() const final;
    QString detailledDescription() const final;
    QString inputDescription() const override;
    QString outputDescription() const override;
    QString detailsDescription() const override;

    CT_VirtualAbstractStep* createNewInstance() const final;

protected:

    void declareInputModels(CT_StepInModelStructureManager& manager) final;

    void declareOutputModels(CT_StepOutModelStructureManager& manager) final;

    void compute() final;

private:
    CT_HandleInResultGroupCopy<>                                m_hInResultGroupCopy;
    CT_HandleInStdZeroOrMoreGroup                               m_hInRootGroup;
    CT_HandleInStdGroup<>                                       m_hInGroup;
    CT_HandleInSingularItem<CT_FileHeader>                      m_hInReaderItem;
    CT_HandleInSingularItem<>                                   m_hInConditionnalItem;

    bool                                    _conditionnal;
    int                                     m_readerAutoIndex;
    int                                     m_totalReaderProgress;
    float                                   m_currentReaderProgress;

    CT_AbstractReader*                      m_readerPrototype;

private slots:
    void readerProgressChanged(int p);
};

#endif // PB_STEPUSEREADERTOLOADFILES_H
