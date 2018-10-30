#ifndef IOBJECTSVISITOR_H
#define IOBJECTSVISITOR_H

class IChunk;

/**
 * @brief This class can be used to visit any type of object
 */
class IObjectsVisitor {
public:
    virtual ~IObjectsVisitor() {}

    /**
     * @brief Returns true if we must continue the visit, false otherwise
     */
    virtual bool mustContinueVisit() const = 0;

    /**
     * @brief Set the chunk to visit
     */
    virtual void visitChunk(const IChunk* chunk) = 0;
};

#endif // IOBJECTSVISITOR_H
