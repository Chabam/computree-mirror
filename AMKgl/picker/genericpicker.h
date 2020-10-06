#ifndef GENERICPICKER_H
#define GENERICPICKER_H

#include <QObject>

#if defined(_WIN32) && defined(_MSC_VER) // Microsoft Visual Studio Compiler
#elif (defined(__linux__) || defined(_WIN32)) && defined(__GNUC__) // GNU Compiler (gcc,g++) for Linux, Unix, and MinGW (Windows)
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#if __GNUC__ > 7
#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#else
#pragma GCC diagnostic ignored "-Wattributes" // See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=89325
#endif
#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#elif defined(__APPLE__) // Clang Compiler (Apple)
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#endif
#include "Eigen/Dense"
#if defined(_WIN32) && defined(_MSC_VER)
#elif (defined(__linux__) || defined(_WIN32)) && defined(__GNUC__)
#pragma GCC diagnostic warning "-Wall"
#pragma GCC diagnostic warning "-Wextra"
#if __GNUC__ > 7
#pragma GCC diagnostic warning "-Wdeprecated-copy"
#else
#pragma GCC diagnostic warning "-Wattributes" // See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=89325
#endif
#pragma GCC diagnostic warning "-Wint-in-bool-context"
#elif defined(__APPLE__)
#pragma GCC diagnostic warning "-Wall"
#pragma GCC diagnostic warning "-Wextra"
#pragma GCC diagnostic warning "-Wint-in-bool-context"
#endif

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
