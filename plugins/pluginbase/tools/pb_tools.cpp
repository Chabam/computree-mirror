#include "pb_tools.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QVariant>

#include <QDebug>

QString PB_Tools::replaceSpecialCharacters(const QString &name)
{
        QString value = replaceAccentCharacters(name);
        value.replace(QRegExp("[\\W]"), "_");
        return value;
}

QString PB_Tools::replaceAccentCharacters(const QString &name)
{
        QString value = name;
        value.replace(QRegExp("[àáâãäå]"), "a");
        value.replace(QRegExp("[ÀÁÂÃÄÅ]"), "A");
        value.replace(QRegExp("[éèëê]"), "e");
        value.replace(QRegExp("[ÈÉÊË]"), "E");
        value.replace(QRegExp("[ìíîï]"), "i");
        value.replace(QRegExp("[ÌÍÎÏ]"), "I");
        value.replace(QRegExp("[òóôõöø]"), "o");
        value.replace(QRegExp("[ÒÓÔÕÖØ]"), "O");
        value.replace(QRegExp("[ùúûü]"), "u");
        value.replace(QRegExp("[ÙÚÛÜ]"), "U");
        value.replace(QRegExp("[ñ]"), "n");
        value.replace(QRegExp("[Ñ]"), "N");
        value.replace(QRegExp("[ç]"), "c");
        value.replace(QRegExp("[Ç]"), "C");
        return value;
}


void PB_Tools::renameDuplicates(QMap<QString, QString> &names, const QMap<QString, QString> &itemNames)
{
   QMultiMap<QString, QString> keysForNames;

   QMapIterator<QString, QString> it(names);
   while (it.hasNext())
   {
       it.next();
       QString key = it.key();
       QString fieldName = it.value();

       keysForNames.insert(fieldName, key);
   }

   for (QString nm : keysForNames.uniqueKeys())
   {
       QList<QString> keysForValue = keysForNames.values(nm);

       if (keysForValue.size() > 1)
       {
           QList<QString> newNames;
           for (QString key : keysForValue)
           {
               QString newName = QString("%1_%2").arg(nm, itemNames.value(key));

               int cpt = 1;
               for (int i = 0 ; i < newNames.size() ; i++)
               {
                   if (newName == newNames.at(i)) {++cpt;}
               }

               newNames.append(newName);
               if (cpt > 1) {newName.append(QString("_%1").arg(cpt));}

               names.insert(key, newName);
           }
       }
   }
}


QMap<QString, QString> PB_Tools::computeShortNames(const QMap<QString, QString> &names)
{
    QMap<QString, QString> shortNames;

    QMultiMap<QString, QString> keysForNames;

    QMapIterator<QString, QString> it(names);
    while (it.hasNext())
    {
        it.next();
        QString key = it.key();
        QString fieldName = it.value();

        QString newName = fieldName.left(10);

        shortNames.insert(key, newName);
        keysForNames.insert(newName, key);
    }

    for (QString nm : keysForNames.uniqueKeys())
    {
        QList<QString> keysForValue = keysForNames.values(nm);

        if (keysForValue.size() > 1)
        {
            QString number = QVariant(keysForValue.size()).toString();
            nm = nm.left(10 - number.length() - 1);

            int cpt = 1;
            for (QString key : keysForValue)
            {
                QString newName = QString("%1_%2").arg(nm).arg(cpt++);
                shortNames.insert(key, newName);
            }
        }
    }

    return shortNames;
}


void PB_Tools::createCorrespondanceFile(QString fileName, const QMap<QString, QString>  &names, const QMap<QString, QString>  &shortNames)
{
    QFileInfo info(fileName);
    QString folder = info.path();

    if (!folder.isEmpty())
    {
        QFile ffields(QString("%1/%2_fields_names.txt").arg(folder).arg(info.baseName()));
        QTextStream fstream(&ffields);
        if (ffields.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMapIterator<QString, QString> itF(shortNames);
            while (itF.hasNext())
            {
                itF.next();
                QString key = itF.key();
                QString shortName = itF.value();
                QString longName = names.value(key);
                fstream << shortName << "\t";
                fstream << longName << "\n";
            }
            ffields.close();
        }
    }
}
