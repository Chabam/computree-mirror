#ifndef APPLYCUSTOMFUNCTIONTOOBJECTVISITOR_H
#define APPLYCUSTOMFUNCTIONTOOBJECTVISITOR_H

#include "iobjectsvisitor.h"

#include <functional>

/**
 * @brief This visitor can help you to apply a custom function to all objects in the view
 */
class ApplyCustomFunctionToObjectVisitor : public IObjectsVisitor
{
public:
    typedef std::function< bool ()> continueFunction;
    typedef std::function< void (const IChunk* chunk)> visitFunction;

    ApplyCustomFunctionToObjectVisitor();

    /**
     * @brief Set the function that say if the visit must continue or not
     */
    void setContinueFunction(continueFunction f);

    /**
     * @brief Set the function called when new objects is available
     */
    void setVisitFunction(visitFunction f);

    /**
     * @brief Returns true if we must continue the visit, false otherwise
     */
    bool mustContinueVisit() const;

    /**
     * @brief Set the chunk to visit
     */
    void visitChunk(const IChunk* chunk);

private:
    continueFunction    m_continueF;
    visitFunction       m_visitF;
};

#endif // APPLYCUSTOMFUNCTIONTOOBJECTVISITOR_H
