#ifndef CT_GROUPBOX_H
#define CT_GROUPBOX_H

#include "ct_view/ct_genericconfigurablewidget.h"
#include "ct_view/ct_widgetwithvaluereferenceinterface.h"

class QGroupBox;

class CT_GroupBox : public CT_GenericConfigurableWidget, public CT_WidgetWithValueReferenceInterface
{
    Q_OBJECT
    typedef CT_GenericConfigurableWidget SuperClass;

public:
    CT_GroupBox(const QString& title, QWidget *parent = nullptr);

    QString type() const { return metaObject()->className(); }

    /**
     * @brief Redefined because in a group box you cannot add a new group box !
     */
    CT_GroupBox* addNewGroupBox(const QString& title) { Q_UNUSED(title) return nullptr; }

    /**
     * @brief Create the QGroupBox and return it
     */
    QWidget* createWidget(QWidget &parent);

    /**
     * @brief Redefined to call it to all widgets in this group box
     */
    void updateValue();

    /**
     * @brief Redefined to check all widgets in this group box
     */
    bool isValueAndWidgetValueDifferent() const;

    void saveSettings(SettingsWriterInterface& writer) const override;
    bool restoreSettings(SettingsReaderInterface& reader) override;

private:
    QGroupBox* m_gp;

public slots:
    /**
     * @brief Call it to expand or collapse the group box
     */
    void collapseOrExpandGroupBox(bool expand);
};

#endif // CT_GROUPBOX_H
