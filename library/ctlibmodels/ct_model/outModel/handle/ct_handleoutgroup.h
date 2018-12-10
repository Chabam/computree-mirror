#ifndef CT_HANDLEOUTGROUP_H
#define CT_HANDLEOUTGROUP_H

#include "ct_model/handle/ct_handlewithmultiplemodelt.h"
#include "ct_model/outModel/ct_outstdgroupmodel.h"

#include <type_traits>

template<class GroupT>
class CT_HandleOutGroup : public CT_HandleWithMultipleModelT<CT_OutStdGroupModel<GroupT>> {

    using SuperClass = CT_HandleWithMultipleModelT<CT_OutStdGroupModel<GroupT>> ;

public:
    using GroupType = GroupT;

    CT_HandleOutGroup() : SuperClass() {
        static_assert(std::is_convertible<GroupT, IGroupForModel>::value, "CT_HandleOutGroup is only compatible with classes that inherit from IGroupForModel");
    }

    template<typename... Args>
    GroupT* createInstance(Args... args) {
        return new GroupT(args...);
    }
};

#endif // CT_HANDLEOUTGROUP_H
