#ifndef PB_STEPCOMPUTEPOINTMETRICS_H
#define PB_STEPCOMPUTEPOINTMETRICS_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_element/abstract/ct_abstractconfigurableelement.h"

#include "ct_itemdrawable/ct_itemattributelist.h"
#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/abstract/ct_abstractareashape2d.h"
#include "ctliblas/itemdrawable/las/ct_stdlaspointsattributescontainer.h"

class PB_StepComputePointMetrics: public CT_AbstractStep
{
    Q_OBJECT
    using SuperClass = CT_AbstractStep;

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepComputePointMetrics();
    ~PB_StepComputePointMetrics() final;

    QString description() const final;

    QString detailledDescription() const final;

    void savePostSettings(SettingsWriterInterface& writer) const override;
    bool restorePostSettings(SettingsReaderInterface &reader) override;

    CT_VirtualAbstractStep* createNewInstance() const final;

protected:

    void declareInputModels(CT_StepInModelStructureManager& manager) final;

    bool postInputConfigure() final;

    void finalizePostSettings() final;

    void declareOutputModels(CT_StepOutModelStructureManager& manager) final;

    void compute() final;

private:

    CT_HandleInResultGroupCopy<>                                    mInResult;
    CT_HandleInStdZeroOrMoreGroup                                   mInRootGroup;
    CT_HandleInStdGroup<>                                           mInGroup;
    CT_HandleInSingularItem<CT_AbstractItemDrawableWithPointCloud>  mInItem;
    CT_HandleInSingularItem<CT_AbstractAreaShape2D, 0>              mInArea;
    CT_HandleInSingularItem<CT_StdLASPointsAttributesContainer, 0>  mInLasPointsAttributesContainer;

    CT_HandleOutItem<CT_ItemAttributeList>                          mOutAttributeList;

    /**
     * @brief The collection of selected metrics to use in the compute method
     */
    QList<CT_AbstractConfigurableElement *>                         m_selectedXYZMetrics;

};

#endif // PB_STEPCOMPUTEPOINTMETRICS_H