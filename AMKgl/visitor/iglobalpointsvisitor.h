#ifndef IGLOBALPOINTSVISITOR_H
#define IGLOBALPOINTSVISITOR_H

#include <QtOpenGL>

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
#include "Eigen/Core"
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

#include "renderer/ichunk.h"
#include <vector>

/**
 * @brief Interface for visit all global points from a scene that contains Items
 */
class IGlobalPointsVisitor {
public:
    IGlobalPointsVisitor() : m_currentChunk(nullptr) {}
    virtual ~IGlobalPointsVisitor() {}

    /**
     * @brief Returns true if we must continue the visit, false otherwise
     */
    virtual bool mustContinueVisit() const = 0;

    /**
     * @brief Set the current chunk that will call the method addPointsToCompute
     */
    virtual void setChunk(const IChunk* chunk) { m_currentChunk = (IChunk*)chunk; }

    /**
     * @brief Called when a list of points is available
     * @param offset : the offset to apply to points (coordinate system)
     * @param indices : collection of local indices of points to test (the index 0 is the index "firstPointGlobalIndex" in the global cloud)
     * @param firstPointGlobalIndex : the global index of the first point in the global cloud
     */
    virtual void addPointsToCompute(const Eigen::Vector3d &offset,
                                    std::vector<GLuint> &indices,
                                    const size_t& firstPointGlobalIndex) = 0;

    /**
     * @brief Return the current chunk
     */
    IChunk* getChunk() const { return m_currentChunk; }

private:
    IChunk* m_currentChunk;
};

#endif // IGLOBALPOINTSVISITOR_H
