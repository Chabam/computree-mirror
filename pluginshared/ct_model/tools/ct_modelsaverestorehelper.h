#ifndef CT_MODELSAVERESTOREHELPER_H
#define CT_MODELSAVERESTOREHELPER_H

#include "ct_result/model/outModel/abstract/ct_outabstractresultmodel.h"

/**
 * @brief Use this class to save a model in a SettingsGroup. You can use this class only if you save all elements (step, result, etc...) in a script
 *        and reload the script. The model of items and result must have the same uniqueName to be founded.
 */
class PLUGINSHAREDSHARED_EXPORT CT_ModelSaveRestoreHelper
{
public:
    /**
     * @brief Save informations to be able to restore the model when you call the method "restoreOutModelFromSettings"
     * @param writer : the writer to use to save settings
     * @param model : the model that must be founded later
     * @param modelUniqueName : a unique name to be able to search the good model (per example if you save settings for more than one model you must pass a unique name for each model)
     */
    static void saveSettingsOfOutModel(const QObject* caller, SettingsWriterInterface &writer, const CT_OutAbstractModel *model, const QString &modelUniqueName);

    /**
     * @brief Search the model described in the settings and return it if founded
     * @param reader : the reader to use to read settings
     * @param step : the step to use to found model
     * @param modelUniqueName : the unique name specified in the method "saveSettingsOfOutModel"
     * @param existAndFoundedOrNotExist : to true if the model if it was saved and founded or was not saved
     * @return The model if founded and can be restored, NULL otherwise
     */
    static CT_OutAbstractModel* restoreOutModelFromSettings(const QObject* caller, SettingsReaderInterface& reader, const CT_VirtualAbstractStep *step, const QString &modelUniqueName, bool& savedAndFoundedOrNotSaved);

private:
    /**
     * @brief Search the result model that contains the model passed in parameter
     * @param originalModel : the original model to search (you can have it by call "model->lastOriginalModelWithAStep()")
     * @return NULL if not founded, the result model otherwise
     */
    static CT_OutAbstractResultModel* getResultModelThatContainsThisModel(const CT_AbstractModel* originalModel);

    /**
     * @brief Search the step in the hierarchy and return it if founded
     * @param hierarchy : the list of steps
     * @param rootStep : the root step
     * @return Returns the step if founded, NULL otherwise
     */
    static CT_VirtualAbstractStep* searchStepInHierarchy(const QStringList& hierarchy, const CT_VirtualAbstractStep* rootStep);

    /**
     * @brief Search the result model with the specified name in out result model of the step
     * @param resultModelUniqueName : the unique name of the result to search
     * @param pStep : the step where to search the result model
     * @return The result model if founded, NULL otherwise
     */
    static CT_OutAbstractResultModel* searchResultModelInStep(const QString& resultModelUniqueName, const CT_VirtualAbstractStep* pStep);
};

#endif // CT_MODELSAVERESTOREHELPER_H
