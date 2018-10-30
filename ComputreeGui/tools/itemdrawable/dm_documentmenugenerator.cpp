#include "dm_documentmenugenerator.h"

#include "dm_guimanager.h"

DM_DocumentMenuGenerator::DM_DocumentMenuGenerator()
{

}

QMenu* DM_DocumentMenuGenerator::createMenuXXXToDocuments(const QString& title,
                                                          const QObject* receiver,
                                                          const char* slot,
                                                          QList<int> exceptDocumentNumber)
{
    QMenu *menu = new QMenu(title);

    constructMenu(menu, receiver, slot, exceptDocumentNumber);

    return menu;
}

void DM_DocumentMenuGenerator::constructMenu(QMenu* menu,
                                             const QObject *receiver,
                                             const char *slot,
                                             QList<int> exceptDocumentNumber)
{
    QList<DocumentInterface*> documents = GUI_MANAGER->getDocumentManagerView()->documents();
    QListIterator<DocumentInterface*> it(documents);

    while(it.hasNext())
    {
        DM_DocumentView *doc = (DM_DocumentView*)it.next();

        if(doc->isVisible()
                && !exceptDocumentNumber.contains(doc->getNumber()))
            addActionToMenu(menu, receiver, slot, doc);
    }
}

void DM_DocumentMenuGenerator::addActionToMenu(QMenu *menu, const QObject *receiver, const char *slot, DM_DocumentView *doc)
{
    QAction* action = menu->addAction(QString("%1").arg(doc->getNumber()));
    action->setData(doc->getNumber());
    QObject::connect(action, SIGNAL(triggered()), receiver, slot);
}
