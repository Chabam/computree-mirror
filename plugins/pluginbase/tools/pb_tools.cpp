#include "pb_tools.h"

#include <QFile>
#include <QTextStream>
#include <QVariant>

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

QMap<QString, QString> PB_Tools::computeShortNames(const QMap<QString, QString> &names)
{
   QMap<QString, QString> shortNames;
   QList<QString> existing;

   QMapIterator<QString, QString> it(names);
   while (it.hasNext())
   {
       it.next();
       QString key = it.key();
       QString value = it.value();

       if (value.size() <= 10)
       {
           shortNames.insert(key, value);
           existing.append(value.toLower());
       } else {
           QString newValue = value.left(10);
           int cpt = 2;
           while (existing.contains(newValue.toLower()))
           {
               QString number = QVariant(cpt++).toString();
               newValue = QString("%1%2").arg(value.left(10 - number.length())).arg(number);
           }
           shortNames.insert(key, newValue);
           existing.append(newValue.toLower());
       }
   }

   return shortNames;
}


void PB_Tools::createCorrespondanceFile(QString folder, const QMap<QString, QString>  &names)
{
    if (!folder.isEmpty())
    {
        QFile ffields(QString("%1/fields_names.txt").arg(folder));
        QTextStream fstream(&ffields);
        if (ffields.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMapIterator<QString, QString> itF(names);
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
