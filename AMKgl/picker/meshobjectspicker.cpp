#include "meshobjectspicker.h"

MeshObjectsPicker::MeshObjectsPicker() : GenericPicker()
{
    m_selectionOperation = nullptr;
    m_checkOperation = nullptr;
    m_globalObjectProvider = NULL;
    m_globalPCProvider = NULL;
}

bool MeshObjectsPicker::isValid() const
{
    return (GenericPicker::isValid()
                && (getSelectionOperation() != nullptr)
                && (getCheckOperation() != nullptr)
                && (m_globalObjectProvider != NULL)
                && (m_globalPCProvider != NULL));
}

void MeshObjectsPicker::setSelectionOperation(selectionOperation op)
{
    m_selectionOperation = op;
}

MeshObjectsPicker::selectionOperation MeshObjectsPicker::getSelectionOperation() const
{
    return m_selectionOperation;
}

void MeshObjectsPicker::setCheckOperation(MeshObjectsPicker::checkOperation op)
{
    m_checkOperation = op;
}

MeshObjectsPicker::checkOperation MeshObjectsPicker::getCheckOperation() const
{
    return m_checkOperation;
}

void MeshObjectsPicker::setGlobalPointCloudAttributesProvider(const PointCloudAttributesProvider *provider)
{
    m_globalPCProvider = (PointCloudAttributesProvider*)provider;
}

PointCloudAttributesProvider* MeshObjectsPicker::getGlobalPointCloudAttributesProvider() const
{
    return m_globalPCProvider;
}

void MeshObjectsPicker::setGlobalObjectCloudAttributesProvider(const ICloudAttributesProvider *provider)
{
    m_globalObjectProvider = (ICloudAttributesProvider*)provider;
}

ICloudAttributesProvider* MeshObjectsPicker::getGlobalObjectCloudAttributesProvider() const
{
    return m_globalObjectProvider;
}

GlobalInfoCloud& MeshObjectsPicker::getSelectionArray() const
{
    return *(m_globalObjectProvider->createOrGetInfoCloud());
}
