#ifndef CT_HANDLEINZEROORMOREGROUP_H
#define CT_HANDLEINZEROORMOREGROUP_H

#include "ct_model/handle/ct_handlewithmodelt.h"
#include "ct_model/inModel/ct_inzeroormoregroupmodel.h"

template<class GroupT>
class CT_HandleInZeroOrMoreGroup : public CT_HandleWithModelT<CT_InZeroOrMoreGroupModel> {

    using SuperClass = CT_HandleWithModelT<CT_InZeroOrMoreGroupModel>;

public:
    using GroupType = GroupT;

    enum {
        MinValue = 1,
        MaxValue = -1
    };

    CT_HandleInZeroOrMoreGroup() : SuperClass() {
        static_assert(std::is_convertible<GroupT, IGroupForModel>::value, "CT_HandleInZeroOrMoreGroup is only compatible with classes that inherit from IGroupForModel");
    }
};

#endif // CT_HANDLEINZEROORMOREGROUP_H
