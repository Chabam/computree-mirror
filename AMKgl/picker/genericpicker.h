#ifndef GENERICPICKER_H
#define GENERICPICKER_H

#include <QObject>

#include <Eigen/Dense>

class AnyElementAttributesProvider;

/**
 * @brief Abstract class for a picker
 */
class GenericPicker : public QObject
{
    Q_OBJECT

public:
    GenericPicker();
    virtual ~GenericPicker() {}

    /**
     * @brief Returns true if all elements was set and compute can be called
     */
    virtual bool isValid() const;

    /**
     * @brief Set the provider to use for enable/disable the selection of an object.
     */
    void setAnyElementsAttributesProvider(const AnyElementAttributesProvider* provider);

    /**
     * @brief Returns the provider used to enable/disable the selection of an object.
     */
    AnyElementAttributesProvider* getAnyElementsAttributesProvider() const;

    /**
     * @brief Set the camera position
     */
    void setCameraPosition(const Eigen::Vector3d &position);

    /**
     * @brief Returns the camera position
     */
    const Eigen::Vector3d& getCameraPosition() const;

    /**
     * @brief Set the camera model view matrix
     */
    void setCameraModelViewMatrix(const Eigen::Matrix4d &mvMatrix);

    /**
     * @brief Returns the camera model view matrix
     */
    const Eigen::Matrix4d& getCameraModelViewMatrix() const;

public slots:

    /**
     * @brief Call this method to compute all
     */
    bool compute();

protected:

    /**
     * @brief Inherit this method to make your computing
     */
    virtual bool internalCompute() = 0;

private:
    AnyElementAttributesProvider*   m_provider;
    Eigen::Vector3d                 m_camPosition;
    bool                            m_camPositionSet;
    Eigen::Matrix4d                 m_camModelViewMatrix;
    bool                            m_camModelViewMatrixSet;

    /**
     * @brief Returns true if camera position was set
     */
    bool isCameraPositionSet() const;

    /**
     * @brief Returns true if camera model view matrix was set
     */
    bool isCameraModelViewMatrixSet() const;

signals:
    /**
     * @brief Emmited when picking has finished
     */
    void finished();
};

#endif // GENERICPICKER_H
