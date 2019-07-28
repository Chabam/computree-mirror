#include "shaderstools.h"

#include <QFile>
#include <QTextStream>

bool ShadersTools::staticMergeShadersFromSourceFile(const QStringList &sourceFilePath, QString& finalSourceCode, QString* error)
{
    finalSourceCode = "";

    foreach (const QString& filepath, sourceFilePath) {
        QFile f(filepath);

        if(f.open(QFile::ReadOnly)) {
            QTextStream stream(&f);

            finalSourceCode += stream.readAll();

            f.close();
        } else {
            if(error != nullptr)
                *error = QObject::tr("Unable to open shaders file %1").arg(filepath);

            return false;
        }
    }

    return true;
}
