#ifndef GSTEPMANAGER2_H
#define GSTEPMANAGER2_H

#include <QWidget>
#include <QMutex>
#include <QTimer>
#include <QList>

#include "view/Steps/gstepviewdefault.h"
#include "view/StepResultTreeView/gtreestepcontextmenu.h"

class CDM_StepManager;
class QTreeWidgetItem;

namespace Ui {
class GStepManager2;
}

/**
 * @brief Widget that show the step workflow and enable the user to interact with it
 *        with a context menu.
 */
class GStepManager2 : public QWidget
{
    Q_OBJECT

public:
    explicit GStepManager2(CDM_StepManager& stepManager, QWidget *parent = 0);
    ~GStepManager2();

public slots:
    /**
     * @brief Call to change the configuration that describe the text to display when display the name of a step
     */
    void setStepNameConfiguration(GStepViewDefault::DisplayNameConfigs config);

    /**
     * @brief Call it when you want to add the specified step to the current step and configure it
     */
    void addStepToCurrentStepOrToRootAndConfigure(CT_VirtualAbstractStep* stepToCopy);

    /**
     * @brief Call it when you want to add a open file step taht can open the specified filepath
     */
    void addOpenFileStep(QString filePath);

    /**
     * @brief Call it to execute the specified step or nullptr to execute step that must be
     */
    void executeStep(CT_VirtualAbstractStep *step = nullptr);

    /**
     * @brief Call it to execute the specified step or nullptr to execute step that must be
     */
    void executeOrForwardStepInDebugMode(CT_VirtualAbstractStep *step = nullptr);

    /**
     * @brief Call it to execute the specified step or nullptr to execute step that must be
     */
    void executeOrForwardStepFastInDebugMode(CT_VirtualAbstractStep *step = nullptr);

    /**
     * @brief Call it to execute the specified step or nullptr to execute step that must be
     */
    void executeOrForwardStepAutoInDebugMode(CT_VirtualAbstractStep *step = nullptr);

private:
    Ui::GStepManager2 *ui;

    /**
     * @brief To know the data type inside a QTreeWidgetItem
     */
    enum DataType {
        DT_Step = 0,
        DT_Result = 1,
        DT_Others = -1
    };

    /**
     * @brief Hashtable to find directly the QTreeWidgetItem of a step
     */
    QHash<CT_VirtualAbstractStep*, QTreeWidgetItem*>                            m_steps;

    /**
     * @brief Hashtable to find directly the QTreeWidgetItem of a result
     */
    QHash<CT_AbstractResult*, QTreeWidgetItem*>                                 m_results;

    /**
     * @brief Hashtable to find directly the QTreeWidgetItem of a result that must be added. The QPair contains first the parent step
     *        and second item to add to the parent.
     */
    QHash<CT_AbstractResult*, QPair<CT_VirtualAbstractStep*, QTreeWidgetItem*> >m_resultsToAdd;

    /**
     * @brief The step manager
     */
    CDM_StepManager&                                                            m_stepManager;

    /**
     * @brief Describe the text to display when display the name of a step
     */
    GStepViewDefault::DisplayNameConfigs                                        m_stepNameConfig;

    /**
     * @brief A mutex to protect datas when a step or a result is added/removed by a non-gui thread
     */
    QMutex*                                                                     m_protectTreeMutex;

    /**
     * @brief A timer that will call the method "updateStepTree" every x ms
     */
    QTimer                                                                      m_updateTreeTimer;

    /**
     * @brief The context menu
     */
    GTreeStepContextMenu*                                                       m_contextMenu;

    QList<QTreeWidgetItem*>                                                     m_itemsToDelete;

    /**
     * @brief Extract the step from the item. Can be nullptr !
     */
    static CT_VirtualAbstractStep* extractStepFromItem(QTreeWidgetItem* item);

    /**
     * @brief Extract the step from the item. Can be nullptr !
     */
    static CT_AbstractResult* extractResultFromItem(QTreeWidgetItem* item);

    /**
     * @brief Set the step to the item.
     */
    static void setStepToItem(CT_VirtualAbstractStep* step, QTreeWidgetItem* item);

    /**
     * @brief Set the result to the item.
     */
    static void setResultToItem(CT_AbstractResult* result, QTreeWidgetItem* item);

    /**
     * @brief Called in "updateTree" first and after for all children that was not deleted
     */
    void recursiveUpdateTree(QTreeWidgetItem* parent);

    /**
     * @brief Called in "updateTree" to add results to parent in the tree in the gui thread
     */
    void addResultsThatMustBe();

    /**
     * @brief Call to create the item for the step and add it to the specified parent
     */
    QTreeWidgetItem* createAndAddItemForStep(CT_VirtualAbstractStep *step, QTreeWidgetItem* parent, int insertRow = -1);

    /**
     * @brief Call to create the item for the result and return it
     */
    QTreeWidgetItem* createItemForResult(const CT_AbstractResult* result);

    /**
     * @brief Call to update all columns of a treewidget that contains a result
     */
    bool updateResultInTree(QTreeWidgetItem *child);

    /**
     * @brief Call to extract information of a result in the current thread (gui or not).
     * @return false if the step was removed. true if all information was extracted.
     */
    bool extractResultInformationFromTree(QTreeWidgetItem *child, QString &name, bool &isClearedFromMemory, bool& isBusy, bool& hasModel, bool& isVisibleInItemModelView);

    /**
     * @brief Call to update all columns of a treewidget that contains a step
     */
    bool updateStepInTree(QTreeWidgetItem *child);

    /**
     * @brief Call to extract information of a step in the current thread (gui or not).
     * @return false if the step was removed. true if all information was extracted.
     */
    bool extractStepInformationFromTree(QTreeWidgetItem *child, QString &name, QString &filepath, int& progress, bool &isSettingsModified, bool &debugModeOn, bool& isManual, int &executeTime);

    /**
     * @brief Call to update the name column of a treewidget that contains a step
     */
    void updateStepNameInTree(QTreeWidgetItem *child, QString name, const bool &isSettingsModified, const int &progress, const QString &filepath);

    /**
     * @brief Call to update the debug column of a treewidget that contains a step
     */
    void updateStepDebugInTree(QTreeWidgetItem *child, bool debugModeOn);

    /**
     * @brief Call to update the execute time column of a treewidget that contains a step
     */
    void updateStepExecuteTimeInTree(QTreeWidgetItem *child, int executeTime);

    /**
     * @brief Call to update the colors of columns of a treewidget that contains a step
     */
    void updateStepColorsInTree(QTreeWidgetItem* child, bool isManual);

    /**
     * @brief Expand or Collapse all item that is the same type as the current item
     */
    void expandOrCollapseAllTypeSameAsCurrentItem(bool expand);

    /**
     * @brief Check/Uncheck the item
     */
    void userCheckItem(QTreeWidgetItem* item, bool checked);

    void deleteItems();

private slots:
    /**
     * @brief Called when user want to show the context menu
     */
    void showContextMenu(const QPoint& p);

    /**
     * @brief Called when user click on a item
     */
    void itemClicked(QTreeWidgetItem* item, int column);

    /**
     * @brief Called when user double click on a item
     */
    void itemDoubleClicked(QTreeWidgetItem* item, int column);

    /**
     * @brief Called when the current item has changed
     */
    void currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);

    /**
     * @brief Call it to remove all root step
     */
    void removeAllStepInWorkflow();

    /**
     * @brief Expand the current item
     */
    void expandCurrentItem();

    /**
     * @brief Collapse the current item
     */
    void collapseCurrentItem();

    /**
     * @brief Expand all item that is the same type as the current item
     */
    void expandAllTypeSameAsCurrentItem();

    /**
     * @brief Collapse all item that is the same type as the current item
     */
    void collapseAllTypeSameAsCurrentItem();

    /**
     * @brief Connected directly to the signal of the stepmanager
     */
    void stepAdded(CT_VirtualAbstractStep* step);

    /**
     * @brief Connected directly to the signal of the stepmanager
     */
    void stepInserted(int row, CT_VirtualAbstractStep* step);

    /**
     * @brief Connected directly to the signal of the stepmanager
     */
    void stepToBeRemoved(CT_VirtualAbstractStep* step);

    /**
     * @brief Connected directly to the signal of the step
     */
    void resultAdded(const CT_AbstractResult* result);

    /**
     * @brief Connected directly to the signal of the step
     */
    void resultToBeRemoved(const CT_AbstractResult* result);

    /**
     * @brief Called by the timer to update the tree
     */
    void updateTree();

signals:
    /**
     * @brief Emitted when gui must locate the step in the menu of steps
     */
    void locateStepInMenu(CT_VirtualAbstractStep* step);

    /**
     * @brief Emitted when the current step has changed
     */
    void currentStepChanged(CT_VirtualAbstractStep* step);
};

#endif // GSTEPMANAGER2_H
