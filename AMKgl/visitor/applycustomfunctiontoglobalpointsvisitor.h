#ifndef APPLYCUSTOMFUNCTIONTOGLOBALPOINTSVISITOR_H
#define APPLYCUSTOMFUNCTIONTOGLOBALPOINTSVISITOR_H

#include "iglobalpointsvisitor.h"

/**
 * @brief This visitor can help you to apply a custom function to all points in the view
 */
class ApplyCustomFunctionToGlobalPointsVisitor : public IGlobalPointsVisitor
{
public:
    /**
     * Custom function to apply to items.
     */
    typedef std::function<void (IChunk* chunk,
                                const Eigen::Vector3d &offset,
                                std::vector<GLuint> &indices,
                                const size_t& firstPointGlobalIndex)> customFunction;

    /**
     * Custom function to know if we must continue or not the visit.
     */
    typedef std::function<bool ()> continueVisitFunction;

    ApplyCustomFunctionToGlobalPointsVisitor();

    /**
     * @brief Call this method and set the your custom function.
     * @example If i want to select all items :
     *
     *          ApplyCustomFunctionToGlobalPointsVisitor visitor;
     *          visitor.setFunction([](IChunk* chunk,
                                       const Eigen::Vector3d &offset,
                                       std::vector<GLuint> &indices,
                                       const size_t& firstPointGlobalIndex) {
     *              // do something
     *          } );
     *
     *          scene.visitPoints(visitor);
     */
    void setFunction(customFunction f);

    /**
     * @brief Call this method to set the function that say if we must continue or not the visit
     */
    void setContinueVisitFunction(continueVisitFunction f);

    // -------- IGlobalPointsVisitor ---------- //
    bool mustContinueVisit() const;
    void addPointsToCompute(const Eigen::Vector3d &offset,
                            std::vector<GLuint> &indices,
                            const size_t& firstPointGlobalIndex);

private:
    customFunction                  m_setFunction;
    continueVisitFunction           m_continueVisitFunction;
};

#endif // APPLYCUSTOMFUNCTIONTOGLOBALPOINTSVISITOR_H
