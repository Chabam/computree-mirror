#ifndef CT_ITEMDRAWABLECOLLECTIONHIERARCHYSTEP_H
#define CT_ITEMDRAWABLECOLLECTIONHIERARCHYSTEP_H

#include "ct_itemdrawablecollectionhierarchyresult.h"

class CT_VirtualAbstractStep;

/**
 * @brief A collection of results (that contains itemdrawable that can be exported) from a step
 */
class CTLIBSTRUCTUREWIDGET_EXPORT CT_ItemDrawableCollectionHierarchyStep
{
public:
    CT_ItemDrawableCollectionHierarchyStep() { step = NULL; }

    CT_VirtualAbstractStep                              *step;
    QList< CT_ItemDrawableCollectionHierarchyResult >   results;
};

#endif // CT_ITEMDRAWABLECOLLECTIONHIERARCHYSTEP_H
