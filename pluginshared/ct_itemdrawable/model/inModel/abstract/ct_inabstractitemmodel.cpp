#include "ct_inabstractitemmodel.h"

CT_InAbstractItemModel::CT_InAbstractItemModel(const QString &uniqueName,
                                               const QString &description,
                                               const QString &displayableName) : SuperClass(uniqueName,
                                                                                                    description,
                                                                                                    displayableName)
{
    m_itemType = "";
    m_itemShortType = "";
}

QString CT_InAbstractItemModel::itemType() const
{
    return m_itemType;
}

QString CT_InAbstractItemModel::itemShortType() const
{
    return m_itemShortType;
}

void CT_InAbstractItemModel::saveSettings(SettingsWriterInterface &writer) const
{
    SuperClass::saveSettings(writer);

    writer.addParameter(this, "ItemType", itemType());
}

bool CT_InAbstractItemModel::restoreSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restoreSettings(reader))
        return false;

    QVariant value;
    if(!reader.parameter(this, "ItemType", value) || (value.toString() != itemType()))
        return false;

    return true;
}

void CT_InAbstractItemModel::setItemType(const QString &itemType)
{
    Q_ASSERT_X(!itemType.isEmpty(), "setItemType", "You pass a item type empty !");

    m_itemType = itemType;
    m_itemShortType = m_itemType.split("/").last();
}

