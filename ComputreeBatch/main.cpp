#include <QtGui>
#include <QApplication>
#include <QMessageBox>

#include "batchdialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("ComputreeBatch");
    QCoreApplication::setApplicationVersion("1.0");

    Batch batch;
    batch.processArgs(app);

    if(!batch.canInitWithArgs())
        return 0;

    if(!batch.mustBeFullBatch()) {

        BatchDialog dialog(batch);
        dialog.show();

        return app.exec();
    }

    QEventLoop loop;
    QObject::connect(&batch, SIGNAL(mustQuitApplication()), &loop, SLOT(quit()), Qt::DirectConnection);

    if(!batch.initWithArgs())
        return 0;

    loop.exec();

    return 0;
}

