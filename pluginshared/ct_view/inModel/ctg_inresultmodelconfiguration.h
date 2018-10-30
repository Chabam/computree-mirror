#ifndef CTG_INRESULTMODELCONFIGURATION_H
#define CTG_INRESULTMODELCONFIGURATION_H

#include <QDialog>
#include <QString>

class CT_InResultModelManager;
class CT_InAbstractResultModel;
class CT_InStdResultModelPossibility;

namespace Ui {
class CTG_InResultModelConfiguration;
}

/**
 * @brief Dialog that present all input models and their possibilities and let the user choose
 *        which possibility use with which input model.
 */
class CTG_InResultModelConfiguration : public QDialog
{
    Q_OBJECT
    
public:
    explicit CTG_InResultModelConfiguration(QWidget* parent = 0);
    ~CTG_InResultModelConfiguration();
    
    /**
     * @brief Set the input result models manager to use
     */
    void setInResultModelManager(const CT_InResultModelManager* manager);

    /**
     * @brief Set if the dialog must be read only or not
     * @param enabled : true if you want that it was read only so the user cannot change anything.
     */
    void setReadOnly(bool enabled);

private:
    Ui::CTG_InResultModelConfiguration  *ui;

    /**
     * @brief Manager of input result models
     */
    CT_InResultModelManager*                       m_inputResultModelManager;

public slots:
    /**
     * @brief Overrided to display a message box if somthing is wrong in configuration and to disable the close action of the dialog
     */
    void accept() override;
};

#endif // CTG_INRESULTMODELCONFIGURATION_H
