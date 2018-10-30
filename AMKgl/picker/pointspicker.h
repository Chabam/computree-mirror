#ifndef POINTSPICKER_H
#define POINTSPICKER_H

#include <vector>
#include <functional>

#include "genericpicker.h"
#include "visitor/iglobalpointsvisitor.h"

#include "tools/pointcloudattributesprovider.h"

#include "definepoint.h"
#include "elementinfo.h"

using namespace AMKgl;

/**
 * @brief Interface of a picker for points
 */
class PointsPicker : public GenericPicker, public IGlobalPointsVisitor
{
public:
    /**
     * @brief Check operation : must return true if we must check the point or not. It was called for each
     *        point. If the method returns false the compute will stop and return the current result.
     */
    typedef std::function<bool (const FloatPoint &p,
                                ElementInfo* firstInfo,
                                const size_t &indexContainedInSelection)> checkOperation;

    /**
     * @brief Selection operation : called when a point must be selected (because it was included in the selection shape)
     */
    typedef std::function<void (const FloatPoint &p,
                                ElementInfo* firstInfo,
                                const size_t &indexContainedInSelection)> selectionOperation;

    PointsPicker();
    virtual ~PointsPicker() {}

    /**
     * @brief Returns true if all elements was set and compute can be called
     */
    virtual bool isValid() const;

    /**
     * @brief Returns true if we must continue the visit, false otherwise
     */
    bool mustContinueVisit() const { return true; }

    /**
     * @brief Set the selection operation
     */
    void setSelectionOperation(selectionOperation op);

    /**
     * @brief Returns the current operation
     */
    selectionOperation getSelectionOperation() const;

    /**
     * @brief Set the operation that returns true if the point must be compute, false otherwise
     */
    void setCheckOperation(checkOperation op);

    /**
     * @brief Returns the check operation
     */
    checkOperation getCheckOperation() const;

    /**
     * @brief Set the provider that manage global point cloud
     */
    void setGlobalPointCloudAttributesProvider(const PointCloudAttributesProvider* provider);

    /**
     * @brief Returns the provider that manage global point cloud
     */
    PointCloudAttributesProvider* getGlobalPointCloudAttributesProvider() const;

private:
    selectionOperation              m_selectionOperation;
    checkOperation                  m_checkOperation;
    PointCloudAttributesProvider*   m_globalPCProvider;

protected:
    /**
     * @brief Returns the cloud to use for selection
     */
    GlobalInfoCloud& getSelectionArray() const;
};

#endif // POINTSPICKER_H
