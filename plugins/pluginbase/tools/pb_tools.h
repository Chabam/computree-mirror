#ifndef PB_TOOLS_H
#define PB_TOOLS_H

#include <QString>
#include <QMap>

class PB_Tools
{
public:    
    static QString replaceSpecialCharacters(const QString &name);
    static QString replaceAccentCharacters(const QString &name);

    static void renameDuplicates(QMap<QString, QString> &names, const QMap<QString, QString> &itemNames);
    static QMap<QString, QString> computeShortNames(const QMap<QString, QString> &names);
    static void createCorrespondanceFile(QString fileName, const QMap<QString, QString>  &names, const QMap<QString, QString> &shortNames);
};

#endif // PB_TOOLS_H
