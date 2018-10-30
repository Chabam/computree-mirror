#include "chunkcustomupdatevalues.h"

ChunkCustomUpdateValues::ChunkCustomUpdateValues()
{
}

void ChunkCustomUpdateValues::setUserValue(const QString &str, const QVariant &v)
{
    m_values.insert(str, v);
}

QVariant ChunkCustomUpdateValues::getUserValue(const QString &str,  const QVariant &defaultV) const
{
    return m_values.value(str, defaultV);
}
