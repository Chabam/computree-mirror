#ifndef GDOCUMENTVIEWFORITEMMODEL_H
#define GDOCUMENTVIEWFORITEMMODEL_H

#include "view/DocumentView/ItemModelViews/gitemmodelview.h"

#include "gdocumentview.h"
#include "view/Tools/gcomboboxwithemptytitlewidget.h"

#include "dm_itemdrawablemanageroptions.h"

#include "dm_itemmodelviewsynchronizedgroup.h"

#include <QTreeView>
#include <QLabel>
#include <QTimer>
#include <QCompleter>

class GDocumentViewForItemModel : public GDocumentView
{
    Q_OBJECT

public:
    GDocumentViewForItemModel(GDocumentManagerView &manager, QString title);
    virtual ~GDocumentViewForItemModel();

    void init();

    QString getType() const;

    void addView(GItemModelView *view);
    const QList<GItemModelView*>& getItemModelViewList() const;

    void beginAddMultipleItemDrawable();
    void endAddMultipleItemDrawable();

    void beginRemoveMultipleItemDrawable();
    void endRemoveMultipleItemDrawable();

    QList<InDocumentViewInterface*> views() const;
    void redrawGraphics() {}
    void updateItems(const QList<CT_AbstractItemDrawable*> &items) { Q_UNUSED(items) }

    bool acceptAction(const CT_AbstractAction *action) const;
    bool setCurrentAction(CT_AbstractAction *action, bool deleteAction = true);
    bool setDefaultAction(CT_AbstractAction *action, bool deleteAction = true);
    CT_AbstractAction* currentAction() const;
    CT_AbstractAction* defaultAction() const;
    void removeActions(const QString &uniqueName) const;

private:

    QList<GItemModelView*>  m_views;
    QHBoxLayout             *m_layoutViews;

    /**
     * @brief Return CT_AbstractItemDrawable selected
     */
    //QList<CT_AbstractItemDrawable *> getItemDrawableCorrespondingToRowSelected();

    //void setReferencesToUseOfItemDrawable(const CT_AbstractItemDrawable *item);

    /**
     * @brief Refresh the references list to use
     */
    //void refreshReferences();

    /**
     * @brief Refresh the label who show the references used
     */
    //void refreshLabelReferences();

    /**
     * @brief Return true if 'ref' exist in m_dataReferencesToUse (compare unique name)
     */
    //bool checkIfReferenceExistInReferences(const IItemDataRef *ref, const QList<IItemDataRef*> &refs) const;

    /**
     * @brief (re)Init the context menu
     */
    //void initContextMenu();

    /**
     * @brief Call this method if you want to change color of the QlineEdit used for filter the treeview
     */
    //void setValidColorForLineFilter(bool valid);

private slots:

    /**
     * @brief Called when a new type is added
     */
    //void slotMustAddItem(DM_ItemDrawableType type);

    /**
     * @brief Called when a type is removed
     */
    //void slotMustRemoveItem(DM_ItemDrawableType type);

    /**
     * @brief Called when a CT_AbstractItemDrawable is added to the document
     */
    //void slotAddItemDrawable(CT_AbstractItemDrawable &item);

    /**
     * @brief Called when a CT_AbstractItemDrawable is removed from the document
     */
    //void slotRemoveItemDrawable(CT_AbstractItemDrawable &item);

    /**
     * @brief Called when the button 'Sync with' is clicked
     */
    //void slotSyncWithClicked();

    /**
     * @brief Called when the user changed the index of the combobox of types
     */
    //void slotComboBoxItemActivated(const QString &text);

    /**
     * @brief Called when the index of the combobox of types changed
     */
    //void slotComboBoxItemChanged(const QString &text);

    /**
     * @brief Called when the a index in the treeview is activated
     */
    //void slotActivated(const QModelIndex &index);

    /**
     * @brief Called when the a index in the treeview is clicked
     */
    //void slotClicked(const QModelIndex &index);

    /**
     * @brief Called when the a index in the treeview is double clicked
     */
    //void slotDoubleClicked(const QModelIndex &index);

    /**
     * @brief Called when a index in the treeview is expanded
     */
    //void slotExpanded(const QModelIndex &i);

    /**
     * @brief Called when a index in the treeview is collapsed
     */
    //void slotCollapsed(const QModelIndex &i);

    /**
     * @brief Called when the timer refresh is time out
     */
    //void slotTimerRefreshTimeOut();

    /**
     * @brief Called when options has changed
     */
    //void slotOptionsUpdated(DM_ItemDrawableManagerOptions options);

    /**
     * @brief Called when text changed from line edit filter
     */
    //void slotSetTextFilter();

    /**
     * @brief Called when we must show the context menu
     */
    //void slotShowViewContextMenu(const QPoint &p);

    /**
     * @brief Called when we must use references of CT_AbstractItemDrawable selected
     */
    //void slotUseReferences();

    /**
     * @brief Called when we must select CT_AbstractItemDrawable of QStandardItem selected
     */
    //void slotSelect();

    /**
     * @brief Called when we must de-select CT_AbstractItemDrawable of QStandardItem selected
     */
    //void slotDeSelect();

    /**
     * @brief Called when we must inverse the selection of QStandardItem selected
     */
    //void slotInverseSelection();

    /**
     * @brief Called when we must set automatic color of CT_AbstractItemDrawable selected
     */
    //void slotColorAuto();

    /**
     * @brief Called when we must set unique color of CT_AbstractItemDrawable selected
     */
    //void slotColorSolid();

    /**
     * @brief Called when the timer is finish and we must reset the color of the QlineEdit that filter the treeview
     */
    //void slotResetColorLineFilter();

    /**
     * @brief Called from a QAction to add selected item to the document (data in QAction)
     */
    //void slotAddSelectedToDocument();

    /**
     * @brief Called when a data of a QStandardItem in the model changed (per example when the itemDrawable is selected)
     */
    //void slotItemDataChanged(QStandardItem *item);

    /**
     * @brief Called when we must show the colors options dialog
     */
    //void slotShowColorOptions();

public slots:

    /**
     * @brief Call to (re)construct the model of the TreeView
     */
    //void construct();

    /**
     * @brief Add selected items to the document with number 'number'
     */
    //void addSelectedItemToDocument(const int &number);

/*signals:
    void syncWith(const GDocumentViewForItemModel *view);*/
};

#endif // GDOCUMENTVIEWFORITEMMODEL_H
