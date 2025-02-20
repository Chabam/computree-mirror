#include "gactionsmanager.h"
#include "ui_gactionsmanager.h"

#include "cdm_pluginmanager.h"
#include "dm_actionsmanager.h"
#include "gdocumentmanagerview.h"
#include "dm_actionshandler.h"

#include "ct_actions/abstract/ct_abstractaction.h"

#include <QToolButton>

GActionsManager::GActionsManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GActionsManager)
{
    ui->setupUi(this);

    ui->treeView->setModel(&m_model);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->setFocusPolicy(Qt::NoFocus);

    m_actionsManager = nullptr;
    m_docManager = nullptr;
    m_currentAction = nullptr;
    m_currentDoc = nullptr;

    QString style = QString("background-color: qlineargradient(spread:pad,"
                                                               "x1:0, y1:0, x2:0, y2:1, "
                                                               "stop:0 rgba(242, 242, 242, 255), "
                                                               "stop:1 rgba(228, 228, 228, 255)); "
                            "border-top-width:1px; "
                            "border-top-style: solid; "
                            "border-top-color: rgb(160, 160, 160); "
                            "border-bottom-width:1px; "
                            "border-bottom-style: solid; "
                            "border-bottom-color: rgb(160, 160, 160);");

    ui->treeView->setStyleSheet("QTreeView::item:has-children {"
                                + style +
                                " }"
                                ""
                                "QTreeView::branch:has-children {"
                                + style +
                                " }"
                                ""
                                "QTreeView::branch:closed:has-children {"
                                   "image: url(:/Icones/Icones/stylesheet-branch-closed.png);"
                                "}"
                                "QTreeView::branch:open:has-children {"
                                   "image: url(:/Icones/Icones/stylesheet-branch-open.png);"
                                "}");

    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));
}

GActionsManager::~GActionsManager()
{
    delete ui;
}

void GActionsManager::setActionsManager(const DM_ActionsManager* actionsManager)
{
    if(m_actionsManager != nullptr)
        disconnect(m_actionsManager, nullptr, this, nullptr);

    m_actionsManager = (DM_ActionsManager*)actionsManager;

    if(m_actionsManager != nullptr)
    {
        connect(m_actionsManager, SIGNAL(actionsCollectionChanged()), this, SLOT(setDefaultActionToDocument()));
        connect(m_actionsManager, SIGNAL(actionsCollectionChanged()), this, SLOT(refreshView()));
    }
}

void GActionsManager::setDocumentManagerView(const GDocumentManagerView* docManager)
{
    if(m_docManager != nullptr)
        disconnect(m_docManager, nullptr, this, nullptr);

    m_docManager = (GDocumentManagerView*)docManager;

    if(m_docManager != nullptr)
    {
        connect(m_docManager, SIGNAL(documentActivated(DM_DocumentView*)), this, SLOT(documentActivated(DM_DocumentView*)));
    }
}

void GActionsManager::refreshView()
{
    // create a list of expanded menus
    QMap<QString, bool> states = menuExpandedState();

    m_model.clear();

    if(m_actionsManager != nullptr)
    {
        QMultiMap<QString, CT_AbstractAction*> byType;

        // Create a map of actions sorted by type. Actions added in the map is get from plugins.
        QList<CT_AbstractAction*> actions = m_actionsManager->actionsFromPlugins();
        QListIterator<CT_AbstractAction*> it(actions);

        while(it.hasNext())
        {
            CT_AbstractAction *ac = it.next();
            byType.insert(ac->type(), ac);
        }

        // Create the model of the treeview from the map
        QList<QString> types = byType.uniqueKeys();
        QListIterator<QString> itK(types);

        while(itK.hasNext())
        {
            const QString &type = itK.next();

            QList<CT_AbstractAction*> actionsType = byType.values(type);

            QStandardItem *menu = new QStandardItem(type);
            menu->setEditable(false);
            menu->setTextAlignment(Qt::AlignHCenter);
            menu->setSelectable(false);
            menu->setData(type);

            QListIterator<CT_AbstractAction*> itA(actionsType);

            while(itA.hasNext())
            {
                CT_AbstractAction *act = itA.next();

                if((m_currentDoc != nullptr) && m_currentDoc->acceptAction(act))
                    menu->appendRow(createItemForAction(act));
            }

            if(menu->rowCount() != 0)
            {
                m_model.appendRow(menu);

                ui->treeView->setExpanded(m_model.indexFromItem(menu), states.value(menu->data().toString(), true));
            }
            else
            {
                delete menu;
            }
        }

        // Continue create the model for actions added by step
        actions = m_actionsManager->actionsFromSteps();

        if(!actions.isEmpty())
        {
            QStandardItem *menu = nullptr;

            QListIterator<CT_AbstractAction*> itS(actions);

            while(itS.hasNext()) {
                CT_AbstractAction *act = itS.next();

                if((m_currentDoc != nullptr) && m_currentDoc->acceptAction(act)) {

                    if(menu == nullptr) {
                        menu = new QStandardItem(tr("Manual/Debug mode"));
                        menu->setEditable(false);
                        menu->setTextAlignment(Qt::AlignHCenter);
                        menu->setSelectable(false);
                        menu->setData(menu->text());
                    }

                    menu->appendRow(createItemForAction(act));
                }
            }

            if(menu != nullptr) {
                m_model.appendRow(menu);
                ui->treeView->setExpanded(m_model.indexFromItem(menu), states.value(menu->data().toString(), true));
            }
        }

        // Continue create the model for actions added by gui/core
        actions = m_actionsManager->internalActions();

        if(!actions.isEmpty())
        {
            QStandardItem *menu = nullptr;

            QListIterator<CT_AbstractAction*> itS(actions);

            while(itS.hasNext()) {
                CT_AbstractAction *act = itS.next();

                if((m_currentDoc != nullptr) && m_currentDoc->acceptAction(act)) {

                    if(menu == nullptr) {
                        menu = new QStandardItem(tr("Interne"));
                        menu->setEditable(false);
                        menu->setTextAlignment(Qt::AlignHCenter);
                        menu->setSelectable(false);
                        menu->setData(menu->text());
                    }

                    menu->appendRow(createItemForAction(act));
                }
            }

            if(menu != nullptr) {
                m_model.appendRow(menu);
                ui->treeView->setExpanded(m_model.indexFromItem(menu), states.value(menu->data().toString(), true));
            }
        }
    }
}

void GActionsManager::setDefaultActionToDocument()
{
    if(m_currentDoc != nullptr)
    {
        CT_AbstractAction *ac = m_currentDoc->currentAction();

        if(ac == nullptr)
        {
            ac = m_actionsManager->action("CT_ActionSelectItemDrawableGV");

            if(ac != nullptr)
                m_currentDoc->setCurrentAction(ac);
        }
    }
}

QMap<QString, bool> GActionsManager::menuExpandedState() const
{
    QMap<QString, bool> list;

    QStandardItem *root = m_model.invisibleRootItem();

    int size = root->rowCount();

    for(int i=0; i<size; ++i)
    {
        QStandardItem *menu = root->child(i);
        list.insert(menu->data().toString(), ui->treeView->isExpanded(m_model.indexFromItem(menu)));
    }

    return list;
}

QList<QStandardItem*> GActionsManager::createItemForAction(CT_AbstractAction *ac) const
{
    QList<QStandardItem*> list;

    QStandardItem *item = new QStandardItem(ac->icon(), ac->title());
    item->setToolTip(ac->toolTip());
    item->setWhatsThis(ac->description());
    item->setEditable(false);
    item->setCheckable(true);
    item->setCheckState((m_currentAction != nullptr) ? ((ac->uniqueName() == m_currentAction->uniqueName()) ? Qt::Checked : Qt::Unchecked) : Qt::Unchecked);
    item->setData(QVariant::fromValue((void*)ac));

    list << item;

    return list;
}

void GActionsManager::documentActivated(DM_DocumentView *view)
{
    if((m_currentDoc != nullptr)
            && m_docManager->containsDocument(m_currentDoc))
        disconnect(m_currentDoc, nullptr, this, nullptr);

    m_currentDoc = view;
    m_currentAction = nullptr;

    if(m_currentDoc != nullptr)
    {
        connect(m_currentDoc, SIGNAL(currentActionChanged(CT_AbstractAction*)), this, SLOT(documentCurrentActionChanged(CT_AbstractAction*)));
        m_currentAction = view->currentAction();

        if(m_currentAction == nullptr)
            setDefaultActionToDocument();
    }

    refreshView();
}

void GActionsManager::documentCurrentActionChanged(CT_AbstractAction *action)
{
    DM_DocumentView *doc = (DM_DocumentView*)sender();

    if(doc != m_currentDoc)
    {
        disconnect(doc, nullptr, this, nullptr);
    }
    else
    {
        m_currentAction = action;

        refreshView();
    }
}

void GActionsManager::itemClicked(const QModelIndex &index)
{
    QStandardItem *item = m_model.itemFromIndex(index);

    if(!item->hasChildren())
    {
        CT_AbstractAction *ac = (CT_AbstractAction*)item->data().value<void*>();

        if((m_currentDoc != nullptr)
                && (((m_currentAction != nullptr) && (m_currentAction->uniqueName() != ac->uniqueName()))
                    || (m_currentAction == nullptr)))
        {
            if(!m_currentDoc->setCurrentAction(ac))
                item->setCheckState(Qt::Unchecked);

        }
        else
            item->setCheckState(Qt::Checked);
    }
}
