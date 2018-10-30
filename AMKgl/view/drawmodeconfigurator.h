#ifndef DRAWMODECONFIGURATOR_H
#define DRAWMODECONFIGURATOR_H

#include <QComboBox>

#include "configurablewidget.h"

#include "renderer/drawmode.h"
#include "scene/permanentsceneelementtype.h"

class DrawModeAccessor;

namespace Ui {
class DrawModeConfigurator;
}

/**
 * @brief Use this widget to change the draw mode of a permanent scene
 */
class DrawModeConfigurator : public ConfigurableWidget
{
    Q_OBJECT

public:
    explicit DrawModeConfigurator(QWidget *parent = 0);
    ~DrawModeConfigurator();

    /**
     * @brief Set the accessor to use. The ui will be updated automatically.
     */
    void setDrawModeAccessor(const DrawModeAccessor* ps);

    /**
     * @brief Call it to set all mode choosed to scene
     */
    void updateScene();

    /**
     * @brief Returns false if an error can happened when we want to apply
     *        user configuration.
     */
    bool checkCanApply();

    /**
     * @brief Same as "updateScene()" method
     */
    bool apply();

private:
    Ui::DrawModeConfigurator*   ui;
    DrawModeAccessor*           m_dmAccessor;

    /**
     * @brief Add element in table
     */
    void initUI();

    /**
     * @brief Update element in table
     */
    void updateUI();

    /**
     * @brief Create a combobox with all draw mode and return it
     */
    QComboBox* createNewComboBox() const;

    /**
     * @brief Create a widget for the line "All" in the table and return it
     */
    QWidget* createNewAllModeWidget() const;

private slots:
    /**
     * @brief Called when the user click on a push button to set a draw mode to all elements in the table
     */
    void pushButtonDrawModeToAllClicked();

    /**
     * @brief Call it to set the specified draw mode to all elements in the table
     */
    void setDrawModeToAll(int drawModeIndex);
};

#endif // DRAWMODECONFIGURATOR_H
