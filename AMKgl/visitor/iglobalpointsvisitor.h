#ifndef IGLOBALPOINTSVISITOR_H
#define IGLOBALPOINTSVISITOR_H

#include <QtOpenGL>

#include "Eigen/Core"
#include "renderer/ichunk.h"
#include <vector>

/**
 * @brief Interface for visit all global points from a scene that contains Items
 */
class IGlobalPointsVisitor {
public:
    IGlobalPointsVisitor() : m_currentChunk(NULL) {}
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
