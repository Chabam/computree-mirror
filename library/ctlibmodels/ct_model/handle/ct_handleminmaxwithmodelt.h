#ifndef CT_HANDLEMINMAXWITHMODELT_H
#define CT_HANDLEMINMAXWITHMODELT_H

#include <QtGlobal>

#include "ct_model/handle/ct_handlewithmodelt.h"

template<class ModelType, int min = 1, int max = 1>
class CT_HandleMinMaxWithModelT : public CT_HandleWithModelT<ModelType> {

    typedef CT_HandleWithModelT<ModelType> SuperClass;

public:
    enum {
        MinValue = min,
        MaxValue = max
    };

    CT_HandleMinMaxWithModelT() : SuperClass() {
        constexpr bool testMinAndMaxValid = (min <= max) || (max == -1);
        static_assert(testMinAndMaxValid, "Value of min and max was invalid ! Max must be greather than min or -1 for your handle.");
        static_assert(!testMinAndMaxValid || (min != max) || (min != 0), "Value of max is invalid ! Must be -1 or greather than 0 for your handle.");
    }
};

#endif // CT_HANDLEMINMAXWITHMODELT_H
