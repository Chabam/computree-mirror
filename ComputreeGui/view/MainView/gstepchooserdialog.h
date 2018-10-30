#ifndef GSTEPCHOOSERDIALOG_H
#define GSTEPCHOOSERDIALOG_H

#include "interfaces.h"
#include "ct_step/tools/menu/ct_menulevel.h"

#include <QDialog>

namespace Ui {
class GStepChooserDialog;
}

class GStepManager;
class GStepViewDefault;

class GStepChooserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GStepChooserDialog(QWidget *parent = 0);
    ~GStepChooserDialog();

    void init();

    GStepViewDefault* stepsChooserWidget() const;

private:
    Ui::GStepChooserDialog *ui;
    QAction                 *m_actionAddStepToFavorites;
    QAction                 *m_actionRemoveStepFromFavorites;
    QAction                 *m_actionRaiseStep;
    QAction                 *m_actionLowerStep;
    QMenu                   *m_contextMenuRootLevel;

    CT_MenuLevel *existInFavorites(CT_VirtualAbstractStep *step);

    CT_MenuLevel* existInLevelRecursively(CT_MenuLevel *level, CT_VirtualAbstractStep *step);

protected:
    bool event(QEvent *e);
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
    void closeEvent(QCloseEvent *e);

private slots:
    void editFavorites();
    void loadFavorites();
    void exportFavorites();

    void removeStepFromFavoritesMenu();
    void raiseStepInFavoritesMenu();
    void lowerStepInFavoritesMenu();
    void showSelectedStepInformation();
    void refreshContextMenuOfStep(CT_VirtualAbstractStep *step);
    void refreshContextMenuOfLevel(CT_MenuLevel *level, int typeOfLevel);

public slots:
    /**
     * @brief Call it when the current step has changed so the menu will enabled/disable steps that can be added or not after this step
     */
    void setCurrentStep(CT_VirtualAbstractStep* step);
    void toggleVisibility();

signals:
    void visibilityChanged(bool visible);
    void maximizedChanged(bool maximized);
    void replaceToDefault(bool left);

    /**
     * @brief Emitted when the user want to add the specified step to the current step and configure it
     */
    void addStepToCurrentStepOrToRootAndConfigure(CT_VirtualAbstractStep* step);
};

#endif // GSTEPCHOOSERDIALOG_H
