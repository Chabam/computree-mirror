#ifndef CT_ATTRIBUTESSCALART_H
#define CT_ATTRIBUTESSCALART_H

#include "ct_attributes/abstract/ct_abstractattributesscalar.h"
#include "ct_cloud/ct_standardcloudstdvectort.h"

/**
 *  Attributes of scalar type (int, float, double, etc...)
 */
template<typename SCALAR>
class CT_AttributesScalarT : public CT_AbstractAttributesScalar
{
public:
    /**
     * @brief Create a collection of SCALAR without valid min and max value.
     */
    CT_AttributesScalarT(const size_t& size = 0);

    /**
     * @brief Create a collection of SCALAR and compute the min and max values from the specified collection.
     */
    CT_AttributesScalarT(CT_StandardCloudStdVectorT<SCALAR>* collection);

    /**
     * @brief Create a collection of SCALAR and use the specified min and max value.
     */
    CT_AttributesScalarT(CT_StandardCloudStdVectorT<SCALAR>* collection,
                         const SCALAR& min,
                         const SCALAR& max);

    /**
     * @brief Copy constructor.
     *
     *        What is copied :
     *          - Collection is copied (call "copy()" method) if not nullptr
     *          - Min and max value are copied
     *
     *        What is initialized differently :
     */
    CT_AttributesScalarT(const CT_AttributesScalarT<SCALAR>& other);

    ~CT_AttributesScalarT();

    double dMin() const override;
    double dMax() const override;

    double dValueAt(const size_t &index) const override;

    SCALAR min() const;
    SCALAR max() const;

    void setMin(const SCALAR &min);
    void setMax(const SCALAR &max);

    const SCALAR& valueAt(const size_t &index) const;
    void setValueAt(const size_t &index, const SCALAR& value);

    size_t attributesSize() const override;

private:
    SCALAR                              m_min;
    SCALAR                              m_max;
    CT_StandardCloudStdVectorT<SCALAR>* m_collection;

protected:

    void initMinMax();

    void internalInitMinMax(std::true_type)
    {
        m_min = std::numeric_limits<SCALAR>::max();
        m_max = -std::numeric_limits<SCALAR>::max();
    }

    void internalInitMinMax(std::false_type)
    {
        m_min = std::numeric_limits<SCALAR>::max();
        m_max = std::numeric_limits<SCALAR>::min();
    }

    CT_StandardCloudStdVectorT<SCALAR>* collection() const { return m_collection; }

    static bool staticCompareScalar(const SCALAR &a, const SCALAR &b);
};

template<>
CTLIBCLOUDS_EXPORT void CT_AttributesScalarT<bool>::initMinMax();

#include "ct_attributes/ct_attributesscalart.hpp"

#endif // CT_ATTRIBUTESSCALART_H
