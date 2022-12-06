#ifndef PB_TOOLS_H
#define PB_TOOLS_H

#include <QString>
#include <QMap>

class PB_Tools
{
public:    
    static QString replaceSpecialCharacters(const QString &name);
    static QString replaceAccentCharacters(const QString &name);

    static QMap<QString, QString> computeShortNames(const QMap<QString, QString> &names);
    static void createCorrespondanceFile(QString folder, const QMap<QString, QString>  &names);
};

#endif // PB_TOOLS_H
