#include "genericpicker.h"

GenericPicker::GenericPicker()
{
    m_provider = nullptr;
    m_camPositionSet = false;
    m_camModelViewMatrixSet = false;
}

bool GenericPicker::isValid() const
{
    return ((getAnyElementsAttributesProvider() != nullptr)
                && isCameraPositionSet()
                && isCameraModelViewMatrixSet());
}

void GenericPicker::setAnyElementsAttributesProvider(const AnyElementAttributesProvider *provider)
{
    m_provider = (AnyElementAttributesProvider*)provider;
}

AnyElementAttributesProvider* GenericPicker::getAnyElementsAttributesProvider() const
{
    return m_provider;
}

void GenericPicker::setCameraPosition(const Eigen::Vector3d &position)
{
    m_camPosition = position;
    m_camPositionSet = true;
}

const Eigen::Vector3d &GenericPicker::getCameraPosition() const
{
    return m_camPosition;
}

void GenericPicker::setCameraModelViewMatrix(const Eigen::Matrix4d &mvMatrix)
{
    m_camModelViewMatrix = mvMatrix;
    m_camModelViewMatrixSet = true;
}

const Eigen::Matrix4d &GenericPicker::getCameraModelViewMatrix() const
{
    return m_camModelViewMatrix;
}

bool GenericPicker::compute()
{
    if(!isValid()) {
        emit finished();
        return false;
    }

    bool  ok = internalCompute();

    emit finished();

    return ok;
}

bool GenericPicker::isCameraPositionSet() const
{
    return m_camPositionSet;
}

bool GenericPicker::isCameraModelViewMatrixSet() const
{
    return m_camModelViewMatrixSet;
}
