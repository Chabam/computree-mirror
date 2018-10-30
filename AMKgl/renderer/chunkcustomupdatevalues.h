#ifndef CHUNKCUSTOMUPDATEVALUES_H
#define CHUNKCUSTOMUPDATEVALUES_H

#include <QHash>
#include <QVariant>

class ChunkCustomUpdateValues
{
public:
    ChunkCustomUpdateValues();

    /**
     * @brief Set a user value defined by a string
     */
    void setUserValue(const QString &str, const QVariant &v);

    /**
     * @brief Returns the user value associated to the string
     */
    QVariant getUserValue(const QString &str, const QVariant &defaultV = QVariant()) const;

private:

    QHash<QString, QVariant>    m_values;
};

#endif // CHUNKCUSTOMUPDATEVALUES_H
