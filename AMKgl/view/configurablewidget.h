#ifndef CONFIGURABLEWIDGET
#define CONFIGURABLEWIDGET

#include <QWidget>

class ConfigurableWidget : public QWidget
{
public:
    ConfigurableWidget(QWidget *parent = 0) : QWidget(parent) {}
    virtual ~ConfigurableWidget() {}

    /**
     * @brief Returns false if an error can happened when we want to apply
     *        user configuration.
     */
    virtual bool checkCanApply() = 0;

    /**
     * @brief Apply the user configuration
     */
    virtual bool apply() = 0;
};

#endif // CONFIGURABLEWIDGET

