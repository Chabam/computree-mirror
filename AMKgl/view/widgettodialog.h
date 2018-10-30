#ifndef WIDGETTODIALOG_H
#define WIDGETTODIALOG_H

#include <QDialog>

#include "configurablewidget.h"

class WidgetToDialog
{
public:
    WidgetToDialog();

    /**
     * @brief Add a widget to a dialog with a button box "Apply" / "Cancel"
     */
    static QDialog::DialogCode showWidgetAsDialog(ConfigurableWidget* w) {
        return showWidgetsAsDialog(QList<ConfigurableWidget*>() << w, w->windowTitle());
    }

    /**
     * @brief Add a list of widget to a dialog with a button box "Apply" / "Cancel"
     */
    static QDialog::DialogCode showWidgetsAsDialog(const QList<ConfigurableWidget*>& l, const QString& title = "");
};

#endif // WIDGETTODIALOG_H
