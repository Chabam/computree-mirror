#ifndef GSCRIPTDOCINFODIALOG_H
#define GSCRIPTDOCINFODIALOG_H

#include <QDialog>

namespace Ui {
class GScriptDocInfoDialog;
}

class GScriptDocInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GScriptDocInfoDialog(QString mainPath, QWidget *parent = nullptr);
    ~GScriptDocInfoDialog();

    QString filename();
    QString title();
    QString author();
    QString description();

private slots:
    void on_pb_export_clicked();

    void on_pb_cancel_clicked();

private:
    Ui::GScriptDocInfoDialog *ui;

    QString _mainPath;
};

#endif // GSCRIPTDOCINFODIALOG_H