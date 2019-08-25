#include "ct_abstractpointsattributescontainer.h"

CT_AbstractPointsAttributesContainer::CT_AbstractPointsAttributesContainer() : SuperClass()
{
}

CT_AbstractPointsAttributes* CT_AbstractPointsAttributesContainer::pointsAttributesAt(const int &key) const
{
    return m_att.value(key, nullptr);
}

void CT_AbstractPointsAttributesContainer::internalInsertAttributes(const int &key, const CT_AbstractPointsAttributes *att)
{
    m_att.insert(key, const_cast<CT_AbstractPointsAttributes*>(att));
}

const QHash<int, CT_AbstractPointsAttributes *>& CT_AbstractPointsAttributesContainer::internalAttributes() const
{
    return m_att;
}
