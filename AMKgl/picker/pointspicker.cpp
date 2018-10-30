#include "pointspicker.h"

#include "tools/anyelementattributesprovider.h"
#include "tools/objectattributesprovider.h"

PointsPicker::PointsPicker() : GenericPicker()
{
    m_selectionOperation = nullptr;
    m_checkOperation = nullptr;
    m_globalPCProvider = NULL;
}

bool PointsPicker::isValid() const
{
    return (GenericPicker::isValid()
                && (getSelectionOperation() != nullptr)
                && (getCheckOperation() != nullptr)
                && (m_globalPCProvider != NULL));
}

void PointsPicker::setSelectionOperation(selectionOperation op)
{
    m_selectionOperation = op;
}

PointsPicker::selectionOperation PointsPicker::getSelectionOperation() const
{
    return m_selectionOperation;
}

void PointsPicker::setCheckOperation(PointsPicker::checkOperation op)
{
    m_checkOperation = op;
}

PointsPicker::checkOperation PointsPicker::getCheckOperation() const
{
    return m_checkOperation;
}

void PointsPicker::setGlobalPointCloudAttributesProvider(const PointCloudAttributesProvider *provider)
{
    m_globalPCProvider = (PointCloudAttributesProvider*)provider;
}

PointCloudAttributesProvider* PointsPicker::getGlobalPointCloudAttributesProvider() const
{
    return m_globalPCProvider;
}

GlobalInfoCloud& PointsPicker::getSelectionArray() const
{
    return *(m_globalPCProvider->createOrGetInfoCloud());
}
