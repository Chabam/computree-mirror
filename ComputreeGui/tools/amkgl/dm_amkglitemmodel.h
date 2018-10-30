#ifndef DM_AMKGLITEMMODEL_H
#define DM_AMKGLITEMMODEL_H

#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractitemmodel.h"
#include "ct_result/ct_resultgroup.h"

class DM_AMKglItemModelIterator {
public:
    CT_AbstractItemDrawable* operator *() const { return NULL; }
    DM_AMKglItemModelIterator& operator ++() { return *this; }
    DM_AMKglItemModelIterator& operator --() { return *this; }
    bool operator !=(const DM_AMKglItemModelIterator& other) const { Q_UNUSED(other) return false; }
    bool operator ==(const DM_AMKglItemModelIterator& other) const { Q_UNUSED(other) return false; }
};

class DM_AMKglItemModel {
public:
    typedef DM_AMKglItemModelIterator const_iterator;

    // only for compatibility ! TODO : but must not be maintened !!!
    DM_AMKglItemModel(CT_OutAbstractItemModel* itemModel) {
#ifdef AMKGL_NO_TODO_WARNINGS
        Q_UNUSED(itemModel)
#endif
    }

    DM_AMKglItemModel(const CT_ResultGroup* result,
                      CT_OutAbstractItemModel* itemModel) {
#ifdef AMKGL_NO_TODO_WARNINGS
        Q_UNUSED(itemModel)
        Q_UNUSED(result)
#endif
    }

    const_iterator begin() const { return DM_AMKglItemModelIterator(); }
    const_iterator end() const { return DM_AMKglItemModelIterator(); }

    bool hasSameModelAsItem(const CT_AbstractItemDrawable* item) const { Q_UNUSED(item) return false; }
};

#endif // DM_AMKGLITEMMODEL_H
