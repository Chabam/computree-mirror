#ifndef CT_ATTRIBUTESSCALART_H
#define CT_ATTRIBUTESSCALART_H

#include "ct_attributes/managers/abstract/ct_abstractxattributemanager.h"
#include "ct_attributes/managers/ct_scalarminmaxmanager.h"
#include "ct_cloudindex/abstract/ct_abstractcloudindex.h"
#include "ct_cloudindex/registered/abstract/ct_abstractcloudindexregistered.h"

/**
 *  Attributes of scalar type (int, float, double, etc...)
 */
template<typename SCALAR, typename InheritFrom, typename MANAGER_SCALAR = SCALAR>
class CT_AttributesScalarT : public InheritFrom
{
public:
    /**
     * @brief The visitor receive the global index of the scalar and the scalar as double. The visitor
     *        must returns true if the visit must continue or false to abort it.
     */
    using DVisitor = std::function<bool (const size_t& /*globalIndex*/, double /*value*/)>;

    /**
     * @brief The visitor receive the global index of the attribute and the attribute (SCALAR). The visitor
     *        must returns true if the visit must continue or false to abort it.
     */
    using SVisitor = std::function<bool (const size_t& /*globalIndex*/, SCALAR /*value*/)>;

    CT_AttributesScalarT();

    /**
     * @brief Create a collection of SCALAR and use the specified min and max value.
     */
    CT_AttributesScalarT(CT_CIR cir,
                         CT_AbstractXAttributeManager<MANAGER_SCALAR>& manager,
                         const SCALAR& min,
                         const SCALAR& max);

    /**
     * @brief Create a collection of SCALAR and compute the local min and max value.
     */
    CT_AttributesScalarT(CT_CIR cir,
                         CT_AbstractXAttributeManager<MANAGER_SCALAR>& manager);
    ~CT_AttributesScalarT();

    /**
     * @brief Copy constructor.
     *
     *        What is copied :
     *          - Attributes manager pointer
     *          - Min and max value are copied
     *
     *        What is initialized differently :
     */
    CT_AttributesScalarT(const CT_AttributesScalarT<SCALAR, InheritFrom, MANAGER_SCALAR>& other) = default;
    CT_AttributesScalarT<SCALAR, InheritFrom, MANAGER_SCALAR>& operator=(const CT_AttributesScalarT<SCALAR, InheritFrom, MANAGER_SCALAR>& other) = default;

    // GLOBAL //
    double scalarAsDoubleAt(const size_t& globalIndex) const;

    bool hasBeenSet(const size_t& globalIndex) const;

    size_t numberOfSetValues() const;

    bool visitValuesAsDouble(DVisitor v) const;

    bool hasValues() const;

    double minScalarAsDouble() const;

    double maxScalarAsDouble() const;

    /**
     * @brief Returns the minimum scalar for the total scalar cloud
     */
    SCALAR minScalar() const;

    /**
     * @brief Returns the maximum scalar for the total scalar cloud
     */
    SCALAR maxScalar() const;

    /**
     * @brief Visit all defined scalars of the total scalar cloud
     */
    bool visitValues(SVisitor v) const;

    // LOCAL //
    double scalarAsDoubleAtLocalIndex(const size_t& localIndex) const;

    size_t numberOfSetLocalValues() const;

    bool hasLocalValues() const;

    bool visitLocalValuesAsDouble(DVisitor v) const;

    double minLocalScalarAsDouble() const;

    double maxLocalScalarAsDouble() const;

    /**
     * @brief Returns the minimum scalar for the local scalar cloud
     */
    SCALAR minLocalScalar() const;

    /**
     * @brief Returns the maximum scalar for the local scalar cloud
     */
    SCALAR maxLocalScalar() const;

    /**
     * @brief Modify the local min and max value
     */
    void setLocalMinMax(const SCALAR& min, const SCALAR& max);

    /**
     * @brief Visit all defined scalars of the total scalar cloud
     */
    bool visitLocalValues(SVisitor v) const;

    /**
     * @brief Returns the manager of the total scalar cloud
     */
    CT_AbstractXAttributeManager<MANAGER_SCALAR>* scalarsManager() const;

    virtual quint64 mask() const { return 0; }

    /**
     * @brief Copy scalars of the source cloud for the destination cloud and let a modificator modify the new value to set
     * @param source : the source cloud
     * @param destination : the destination cloud (must be the same size as the source cloud)
     * @param modificator : a function that must modify the new value to set or nullptr to copy it only
     * @return false if destination has values already set ! true otherwise
     */
    bool copyAndModifyAttributesOfSForD(CT_CIR source, CT_CIR destination, typename CT_AbstractXAttributeManager<MANAGER_SCALAR>::AttributeModificator modificator = nullptr) { return scalarsManager()->copyAndModifyAttributesOfSForD(source, destination, modificator); }

private:
    CT_CIR                                          m_cir;
    SCALAR                                          m_min;
    SCALAR                                          m_max;
    CT_AbstractXAttributeManager<MANAGER_SCALAR>*   m_manager;

protected:
    virtual SCALAR convertScalarOfManagerToScalar(const MANAGER_SCALAR& scalar) const
    {
        return internalConvertScalarOfManagerToScalar(scalar, std::integral_constant<bool, std::is_convertible<MANAGER_SCALAR, SCALAR>::value>());
    }

    SCALAR internalConvertScalarOfManagerToScalar(const MANAGER_SCALAR& scalar, std::true_type) const
    {
        return scalar;
    }

    SCALAR internalConvertScalarOfManagerToScalar(const MANAGER_SCALAR&, std::false_type) const
    {
        return SCALAR();
    }
};

#include "ct_attributes/ct_attributesscalart.hpp"

#endif // CT_ATTRIBUTESSCALART_H
