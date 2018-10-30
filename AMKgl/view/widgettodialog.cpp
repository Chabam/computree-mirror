#include "widgettodialog.h"

#include <QVBoxLayout>
#include <QDialogButtonBox>

WidgetToDialog::WidgetToDialog()
{
}

QDialog::DialogCode WidgetToDialog::showWidgetsAsDialog(const QList<ConfigurableWidget *> &l, const QString& title)
{
    QDialog d;

    if(!title.isEmpty())
        d.setWindowTitle(title);

    QSize mSize(0, 0);

    QVBoxLayout* layout = new QVBoxLayout(&d);

    foreach (ConfigurableWidget* w, l) {
        layout->addWidget(w);
        mSize.setHeight(mSize.height() + w->minimumHeight());
        mSize.setWidth(qMax(mSize.width(), w->minimumWidth()));
    }

    QDialogButtonBox* bb = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, Qt::Horizontal);
    layout->addWidget(bb);

    QObject::connect(bb, SIGNAL(accepted()), &d, SLOT(accept()));
    QObject::connect(bb, SIGNAL(rejected()), &d, SLOT(reject()));

    d.setMinimumSize(mSize);

    bool ok = false;

    while(!ok) {
        if(d.exec() == QDialog::Rejected)
            return QDialog::Rejected;

        ok = true;

        foreach (ConfigurableWidget* w, l) {
            if(!w->checkCanApply()) {
                ok = false;
                break;
            }
        }
    }

    foreach (ConfigurableWidget* w, l) {
        w->apply();
    }

    return QDialog::Accepted;
}

