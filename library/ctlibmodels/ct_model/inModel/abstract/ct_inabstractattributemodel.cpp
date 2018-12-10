#include "ct_inabstractitemattributemodel.h"

#include "ct_model/outModel/abstract/ct_outabstractitemattributemodel.h"

CT_InAbstractItemAttributeModel::CT_InAbstractItemAttributeModel(const QString &displayableName) : SuperClass(displayableName),
    m_vType(0)
{
}

CT_InAbstractItemAttributeModel::CT_InAbstractItemAttributeModel(const CT_InAbstractItemAttributeModel& other) : SuperClass(other),
    m_categoriesType(other.m_categoriesType),
    m_vType(other.m_vType)
{
}

void CT_InAbstractItemAttributeModel::setCategoriesType(const QStringList& categories)
{
    m_categoriesType = categories;
}

void CT_InAbstractItemAttributeModel::setValueType(const int& type)
{
    m_vType = type;
}

QList<QString> CT_InAbstractItemAttributeModel::categoriesType() const
{
    return m_categoriesType;
}

int CT_InAbstractItemAttributeModel::valueType() const
{
    return m_vType;
}

bool CT_InAbstractItemAttributeModel::canBeComparedWith(const CT_OutAbstractModel& model) const
{
    return (dynamic_cast<const CT_OutAbstractItemAttributeModel*>(&model) != NULL);
}

bool CT_InAbstractItemAttributeModel::canBeAPossibility(const CT_OutAbstractModel& model) const
{
    const CT_OutAbstractItemAttributeModel* iaModel = dynamic_cast<const CT_OutAbstractItemAttributeModel*>(&model);

    if(iaModel == NULL)
        return false;

    const IItemAttributeForModel* ia = iaModel->itemAttribute();
    const ICategoryForModel* iaCat = ia->itemAttributeToolForModel()->category();

    if(!categoriesType().contains(iaCat->uniqueName()))
    {
        bool ok = false;

        for(const QString& category : m_categoriesType)
        {
            if(iaCat->isEquivalentTo(category)) {
                ok = true;
                break;
            }
        }

        if(!ok)
            return false;
    }

    return ((ia->itemAttributeToolForModel()->valueType() & valueType()) != 0);
}
