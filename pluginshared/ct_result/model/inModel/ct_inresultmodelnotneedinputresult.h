#ifndef CT_INRESULTMODELNOTNEEDINPUTRESULT_H
#define CT_INRESULTMODELNOTNEEDINPUTRESULT_H

#include "ct_result/model/inModel/abstract/ct_inabstractresultmodel.h"

class CT_OutAbstractModel;

/**
 * @brief This class represent an input result model that don't need a matching output result model. Use it if you
 *        don't want to process input results.
 */
class PLUGINSHAREDSHARED_EXPORT CT_InResultModelNotNeedInputResult : public CT_InAbstractResultModel
{
    Q_OBJECT
    typedef CT_InAbstractResultModel SuperClass;

public:
    /**
     * @brief Construct a model. The unique name, the description, etc... cannot be set because it was the same for
     *        all model of the same type. So you cannot add this model twice in the same input result model manager.
     */
    CT_InResultModelNotNeedInputResult();

    /**
     * @brief This model have no childrens
     */
    QList<CT_AbstractModel*> childrens() const override { return QList<CT_AbstractModel*>(); }

    /**
     * @brief This model is always empty
     */
    bool isEmpty() const override { return true; }

    /**
     * @brief This model cannot search in step for results
     */
    QList<CT_AbstractResult*> searchResultsInStepForPossibilitiesSelected(const CT_VirtualAbstractStep *step) const override { Q_UNUSED(step) return QList<CT_AbstractResult*>(); }

    /**
     * @brief This model cannot search in step for results
     */
    CT_AbstractResult* searchResultsInStepForThisPossibility(const CT_VirtualAbstractStep *step, const CT_InStdModelPossibility *possibility) const override { Q_UNUSED(step) Q_UNUSED(possibility) return NULL; }

    /**
     * @brief This model doesn't need output model
     */
    bool needOutputModel() const override { return false; }

    void saveSettings(SettingsWriterInterface& writer) const override;
    bool restoreSettings(SettingsReaderInterface& reader) override;
    CT_InAbstractModel* copy(bool withPossibilities) const override;

    /**
     * @brief This model cannot be compared with an output model
     */
    bool canBeComparedWith(const CT_OutAbstractModel &model) const override { Q_UNUSED(model); return false; }

protected:

    /**
     * @brief This model cannot have possibilities
     */
    bool canAddPossibilityForThisModel(const CT_OutAbstractModel &model) const override { Q_UNUSED(model); return false; }
};

#endif // CT_INRESULTMODELNOTNEEDINPUTRESULT_H
