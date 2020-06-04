#ifndef CT_SCALARMINMAXMANAGER_H
#define CT_SCALARMINMAXMANAGER_H

#include <QHash>

#include <limits>

template<typename SCALAR>
class CT_ScalarMinMaxManager
{
public:
    CT_ScalarMinMaxManager()
    {
        InitMinMax(mMin, mMax);
    }

    template<typename ATT>
    void registerAttribute(ATT* attribute)
    {
        Backup backup{attribute->minLocalScalar(), attribute->maxLocalScalar()};

        mMin = qMin(mMin, backup.mMin);
        mMax = qMax(mMax, backup.mMax);

        mMinMaxAttributes.insert(attribute, backup);
    }

    template<typename ATT>
    void updateAttribute(ATT* attribute)
    {
        Backup& backup = mMinMaxAttributes[attribute];
        backup.mMin = attribute->minLocalScalar();
        backup.mMin = attribute->maxLocalScalar();

        InitMinMax(mMin, mMax);

        for(const Backup& backup : mMinMaxAttributes)
        {
            mMin = qMin(mMin, backup.mMin);
            mMax = qMax(mMax, backup.mMax);
        }
    }

    template<typename ATT>
    void unregisterAttribute(ATT* attribute)
    {
        mMinMaxAttributes.remove(attribute);

        InitMinMax(mMin, mMax);

        for(const Backup& backup : mMinMaxAttributes)
        {
            mMin = qMin(mMin, backup.mMin);
            mMax = qMax(mMax, backup.mMax);
        }
    }

    SCALAR min() const { return mMin; }

    SCALAR max() const { return mMax; }

    static void InitMinMax(SCALAR& min, SCALAR& max)
    {
        InternalInitMinMax(min, max, std::integral_constant<bool, std::numeric_limits<SCALAR>::is_signed>());
    }

private:
    struct Backup
    {
        SCALAR  mMin;
        SCALAR  mMax;
    };

    SCALAR  mMin;
    SCALAR  mMax;

    QHash<void*, Backup> mMinMaxAttributes;

    // TODO
    /*static void InternalInitMinMaxIfBool(SCALAR& min, SCALAR& max, std::true_type)
    {
        min = true;
        max = false;
    }*/

    static void InternalInitMinMax(SCALAR& min, SCALAR& max, std::true_type)
    {
        min = std::numeric_limits<SCALAR>::max();
        max = -std::numeric_limits<SCALAR>::max();
    }

    static void InternalInitMinMax(SCALAR& min, SCALAR& max, std::false_type)
    {
        min = std::numeric_limits<SCALAR>::max();
        max = std::numeric_limits<SCALAR>::min();
    }
};

#endif // CT_SCALARMINMAXMANAGER_H
