#ifndef CT_HANDLEINRESULTCOPY_H
#define CT_HANDLEINRESULTCOPY_H

#include "ct_model/handle/ct_handleminmaxwithmodelt.h"
#include "ct_model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"

template<class ResultT, int min = 1, int max = 1>
class CT_HandleInResultCopy : public CT_HandleMinMaxWithModelT<CT_InResultModelGroupToCopy, min, max> {

    using SuperClass = CT_HandleMinMaxWithModelT<CT_InResultModelGroupToCopy, min, max>;

public:
    using ResultType = ResultT;

    CT_HandleInResultCopy() : SuperClass() {
        static_assert(std::is_convertible<ResultT*, IResultGroupForModel*>::value, "CT_HandleInResultCopy is only compatible with classes that inherit from IResultGroupForModel");
    }
};

#endif // CT_HANDLEINRESULTCOPY_H
