#ifndef DM_DOCUMENTMENUGENERATOR_H
#define DM_DOCUMENTMENUGENERATOR_H

#include <QMenu>
#include <QList>

class DM_DocumentView;

/**
 * @brief Class that you can use to create a menu to add/remove items to document
 */
class DM_DocumentMenuGenerator
{
public:
    DM_DocumentMenuGenerator();

    /**
     * @brief Returns a menu that show a title and add actions for all documents except them specified
     */
    QMenu* createMenuXXXToDocuments(const QString& title, const QObject *receiver, const char *slot, QList<int> exceptDocumentNumber = QList<int>());

private:
    /**
     * @brief Construct the menu
     */
    void constructMenu(QMenu* menu, const QObject *receiver, const char *slot, QList<int> exceptDocumentNumber);

    /**
     * @brief Add the action to the menu
     */
    void addActionToMenu(QMenu* menu, const QObject *receiver, const char *slot, DM_DocumentView* doc);
};

#endif // DM_DOCUMENTMENUGENERATOR_H
