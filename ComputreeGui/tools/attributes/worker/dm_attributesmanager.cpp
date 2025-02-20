#include "dm_attributesmanager.h"

DM_AttributesManager::DM_AttributesManager()
{
}

DM_AttributesManager::~DM_AttributesManager()
{
    qDeleteAll(m_collection.begin(), m_collection.end());
}

void DM_AttributesManager::addAttributes(DM_AbstractAttributes *att)
{
    m_collection.append(att);
    att->moveToThread(&m_thread);
}

void DM_AttributesManager::remove(const int &index)
{
    delete m_collection.takeAt(index);
}

int DM_AttributesManager::size() const
{
    return m_collection.size();
}

void DM_AttributesManager::clearInvalid()
{
    QMutableListIterator<DM_AbstractAttributes*> it(m_collection);

    while(it.hasNext())
    {
        if(it.next()->abstractAttributes() == nullptr)
            it.remove();
    }
}

DM_AbstractAttributes *DM_AttributesManager::getAttributesFromInterface(const CT_AbstractAttributes *ia, bool local) const
{
    QListIterator<DM_AbstractAttributes*> it(m_collection);

    while(it.hasNext())
    {
        DM_AbstractAttributes *da = it.next();

        if((da->abstractAttributes() == ia) && (da->mustApplyToLocalIndex() == local))
            return da;
    }

    return nullptr;
}

DM_AbstractAttributes* DM_AttributesManager::getAttributesFromModel(const CT_OutAbstractModel* model, bool local) const
{
    QListIterator<DM_AbstractAttributes*> it(m_collection);

    while(it.hasNext())
    {
        DM_AbstractAttributes *da = it.next();

        if((da->model() == model) && (da->mustApplyToLocalIndex() == local))
            return da;
    }

    return nullptr;
}

const QList<DM_AbstractAttributes *>& DM_AttributesManager::attributes() const
{
    return m_collection;
}

