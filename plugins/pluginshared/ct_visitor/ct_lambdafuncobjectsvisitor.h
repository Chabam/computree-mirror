#ifndef CT_LAMBDAFUNCOBJECTSVISITOR_H
#define CT_LAMBDAFUNCOBJECTSVISITOR_H

#include "ct_visitor/abstract/ct_abstractobjectsvisitor.h"

#include <functional>

/**
 * @brief A generic visitor that use lambda function to do the job. You must set
 *        the continueFunction and the visitFunction to use this visitor.
 *
 * @example Count number of global points :
 *
 *          size_t count = 0;
 *
 *          CT_LambdaFuncObjectsVisitor visitor;
 *          visitor.setContinueFunction([]() -> bool {
 *                         return true;
 *          });
 *
 *          visitor.setVisitFunction([&](CT_LambdaFuncObjectsVisitor& visitor){
 *              count += visitor.getNumberOfObjects();
 *          });
 *
 *          graphicsView->visitObjects(GOT_PointGlobalCloud, visitor);
 */
class PLUGINSHAREDSHARED_EXPORT CT_LambdaFuncObjectsVisitor : public CT_AbstractObjectsVisitor
{
public:
    typedef std::function< bool ()> continueFunction;
    typedef std::function< void (CT_LambdaFuncObjectsVisitor& visitor)> visitFunction;

    CT_LambdaFuncObjectsVisitor();

    /**
     * @brief Set the function that say if the visit must continue or not
     */
    void setContinueFunction(continueFunction f);

    /**
     * @brief Set the function called when new objects is available
     */
    void setVisitFunction(visitFunction f);

    /**
     * @brief You must return true if you want to continue the visit, false to abort it
     */
    bool continueVisit() const;

    /**
     * @brief Called when new objects is available.
     *
     *        If you want to modify colors/normals/visibility/etc.... you must call the
     *        method "createObjectsModifier" that allow you to do this type of operations.
     *
     *        If you want to know the status of elements you must call the method
     *        "createObjectsStatusQueryior" that allow you to do this type of operations.
     */
    void newObjectsAvailable();

private:
    continueFunction    m_continueF;
    visitFunction       m_visitF;
};

#endif // CT_LAMBDAFUNCOBJECTSVISITOR_H
