#ifndef CT_OUTABSTRACTRESULTMODEL_H
#define CT_OUTABSTRACTRESULTMODEL_H

#include "ct_model/outModel/abstract/ct_outabstractmodel.h"
#include "ct_itemdrawable/model/outModel/abstract/def_ct_abstractgroupmodelout.h"

#include <QList>

class CT_ResultInitializeData;
class CT_AbstractResult;

/**
 * @brief Represent a model that represent a result
 */
class PLUGINSHAREDSHARED_EXPORT CT_OutAbstractResultModel : public CT_OutAbstractModel
{
    typedef CT_OutAbstractModel SuperClass;

public:

    /**
     * @brief Construct a OUTPUT model that represent a result
     * @param uniqueName : a unique name in all the hierarchy
     * @param description : a description
     * @param displayableName : a displayable name
     */
    CT_OutAbstractResultModel(const QString &uniqueName,
                              const QString &description,
                              const QString &displayableName,
                              const QString &resultName = "");

    /**
     * @brief Returns the name that will be set to the generated result
     */
    QString resultName() const;

    /**
     * @brief Create the result and return it.
     * @param data : initialized data for the result
     * @return NULL if it was a problem
     */
    virtual CT_AbstractResult* createResult(CT_ResultInitializeData &data) const = 0;

    /**
     * @brief Return the root model that represent a group
     */
    virtual DEF_CT_AbstractGroupModelOut* rootGroup() const = 0;

    /**
     * @brief Set the root model that represent a group (delete the last root group from memory if exist).
     */
    virtual void setRootGroup(DEF_CT_AbstractGroupModelOut *rootGroup) = 0;

    // INHERITED FROM CT_AbstractModel //
    QList<CT_AbstractModel*> childrens() const override;
    bool isEmpty() const override;
    //                  //

private:
    QString     m_resultName;
};

#endif // CT_OUTABSTRACTRESULTMODEL_H
