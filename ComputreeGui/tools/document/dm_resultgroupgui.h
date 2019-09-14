#ifndef DM_RESULTGROUPGUI_H
#define DM_RESULTGROUPGUI_H

#include "ct_model/outModel/manager/ct_outmodelstructuremanager.h"
#include "ct_result/ct_resultgroup.h"

#include "dm_itemgroupgui.h"

/**
 * @brief This class can be used by a document to simulate a result that contains items
 *        of the document (selected, all, etc...). It will generate his model automatically.
 */
class DM_ResultGroupGui : public CT_ResultGroup
{
    Q_OBJECT
    using SuperClass = CT_ResultGroup;

public:
    class DM_OutModelStructureManager : public CT_OutModelStructureManager
    {
    public:
        void setResultModel(CT_OutAbstractResultModel* resultModel)
        {
            internalAddResultModel(resultModel, true);
        }
    };

    DM_ResultGroupGui();
    DM_ResultGroupGui(const DM_ResultGroupGui& other) = default;

    /**
     * @brief Call this method after create the result
     */
    void init();

    /**
     * @brief Set the graphics view if you want that the model contains color and normal clouds
     *        of the graphics.
     */
    void setGraphicsView(const GraphicsViewInterface* gv);

    /**
     * @brief Add an item. The item will be added to the root group.
     */
    void addItemDrawable(CT_AbstractItemDrawable &item);

    /**
     * @brief Remove an item. The item will be removed from the root group.
     */
    void removeItemDrawable(CT_AbstractItemDrawable &item);

    /**
     * @brief Set attributes that must be added to the root group model.
     */
    void setAttributes(const QList<CT_AbstractAttributes*>& attributes);

    /**
     * @brief Before you will use the DM_OutModelStructureManager you must call this
     *        method. It will set the result to all models and generate unique index, etc...
     *        If you change something to this result you must call this method before use the
     *        DM_OutModelStructureManager !
     */
    void finalizeBeforeUseIt();

    /**
     * @brief Returns the DM_OutModelStructureManager so you can use it with an input model manager.
     *        If you change something to this result you must call the method "finalizeBeforeUseIt" before use this
     *        DM_OutModelStructureManager !
     */
    const DM_OutModelStructureManager* outModelStructureManager() const;

    /**
     * @brief Cannot copy it so returns nullptr
     */
    CT_AbstractResult* copy(const CT_OutAbstractResultModel* newModelToUse) const override;

private:
    DM_ItemGroupGui*            mRootGroup;
    DM_OutModelStructureManager mManager;
    bool                        mFinalized;
};

#endif // DM_RESULTGROUPGUI_H
