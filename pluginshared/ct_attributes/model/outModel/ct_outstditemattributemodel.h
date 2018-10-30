#ifndef CT_OUTSTDITEMATTRIBUTEMODEL_H
#define CT_OUTSTDITEMATTRIBUTEMODEL_H

#include "ct_attributes/model/outModel/abstract/ct_outabstractitemattributemodel.h"

/**
 * @brief Use this class if you will inform that you want create a item attribute and add it to an
 *        other OUTPUT model that represent a singular item
 */
class PLUGINSHAREDSHARED_EXPORT CT_OutStdItemAttributeModel : public CT_OutAbstractItemAttributeModel
{
    Q_OBJECT

public:
    CT_OutStdItemAttributeModel(const QString &uniqueName,
                                CT_AbstractItemAttribute *attribute,
                                const QString &displayableName = "",
                                const QString &description = "");

    virtual CT_OutAbstractModel* copy() const;
};

#endif // CT_OUTSTDITEMATTRIBUTEMODEL_H
