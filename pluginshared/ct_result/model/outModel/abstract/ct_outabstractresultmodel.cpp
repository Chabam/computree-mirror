#include "ct_result/model/outModel/abstract/ct_outabstractresultmodel.h"

CT_OutAbstractResultModel::CT_OutAbstractResultModel(const QString &uniqueName,
                                                     const QString &description,
                                                     const QString &displayableName,
                                                     const QString &resultName) : SuperClass(uniqueName,
                                                                                             description,
                                                                                             displayableName)
{
    m_resultName = resultName;
}

QString CT_OutAbstractResultModel::resultName() const
{
    return m_resultName;
}

QList<CT_AbstractModel *> CT_OutAbstractResultModel::childrens() const
{
    QList<CT_AbstractModel *> l;

    if(rootGroup() != NULL)
        l.append(rootGroup());

    return l;
}

bool CT_OutAbstractResultModel::isEmpty() const
{
    return (rootGroup() == NULL);
}
