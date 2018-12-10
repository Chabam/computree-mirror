#include "ct_abstractsingularitemdrawable.h"

#include "ct_itemdrawable/ct_standarditemgroup.h"
#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"
#include "ct_model/inModel/tools/ct_instdmodelpossibilityselectiongroup.h"

// initialize default item attributes of this class for each unique number declared
CT_DEFAULT_IA_INIT(CT_AbstractSingularItemDrawable)

CT_AbstractSingularItemDrawable::CT_AbstractSingularItemDrawable() : SuperClass()
{
    m_centerCoordinates(0) = 0;
    m_centerCoordinates(1) = 0;
    m_centerCoordinates(2) = 0;

    m_defaultColor = Qt::white;
}

CT_AbstractSingularItemDrawable::CT_AbstractSingularItemDrawable(const CT_AbstractSingularItemDrawable& other) : SuperClass(other),
    m_centerCoordinates(other.m_centerCoordinates),
    m_defaultColor(other.m_defaultColor)
{
    // TODO : MK 20.11.18 copy item attributes
}

void CT_AbstractSingularItemDrawable::setCenterX(double x)
{
    m_centerCoordinates(0) = x;
}

void CT_AbstractSingularItemDrawable::setCenterY(double y)
{
    m_centerCoordinates(1) = y;
}

void CT_AbstractSingularItemDrawable::setCenterZ(double z)
{
    m_centerCoordinates(2) = z;
}

double CT_AbstractSingularItemDrawable::centerX() const
{
    return m_centerCoordinates(0);
}

double CT_AbstractSingularItemDrawable::centerY() const
{
    return m_centerCoordinates(1);
}

double CT_AbstractSingularItemDrawable::centerZ() const
{
    return m_centerCoordinates(2);
}

bool CT_AbstractSingularItemDrawable::visitChildrens(const CT_AbstractItem::ChildrensVisitor& visitor) const
{
    const auto visitorAdapter = [&visitor](const CT_AbstractItemAttribute* attribute) -> bool {
        return visitor(attribute);
    };

    if(!PS_DIAM->visitItemAttributesForType(type(), visitorAdapter))
        return false;

    return m_itemAttributes.visitItemAttributes(visitorAdapter);
}

int CT_AbstractSingularItemDrawable::nChildrens() const
{
    return nItemAttributes();
}

void CT_AbstractSingularItemDrawable::addItemAttributeWithOutModel(const CT_OutAbstractItemAttributeModel* outModel, CT_AbstractItemAttribute* itemAttribute)
{
    Q_ASSERT(itemAttribute != NULL);
    Q_ASSERT(outModel != NULL);
    Q_ASSERT(outModel->parentModel() != NULL);
    Q_ASSERT(static_cast<CT_OutAbstractModel*>(outModel->parentModel())->recursiveOriginalModel() == model()->recursiveOriginalModel());
    Q_ASSERT(outModel->itemAttribute()->itemAttributeToolForModel()->category() == itemAttribute->category());

    itemAttribute->setModel(outModel);
    itemAttribute->setResult(static_cast<CT_AbstractResult*>(outModel->result()));

    m_itemAttributes.addItemAttribute(itemAttribute);
}

CT_AbstractItemAttribute* CT_AbstractSingularItemDrawable::itemAttributeWithOutModel(const CT_OutAbstractItemAttributeModel* outModel) const
{
    Q_ASSERT(outModel != NULL);

    if(outModel->isADefaultItemAttributeModel())
        return PS_DIAM->itemAttributeFromOutModel(outModel, type());

    Q_ASSERT(outModel->parentModel() == model());

    return m_itemAttributes.itemAttributeFromOutModel(outModel);
}

bool CT_AbstractSingularItemDrawable::visitItemAttributes(const CT_ItemAttributeContainer::ItemAttributesVisitor& visitor) const
{
    if(!PS_DIAM->visitItemAttributesForType(type(), visitor))
        return false;

    return m_itemAttributes.visitItemAttributes(visitor);
}

bool CT_AbstractSingularItemDrawable::visitDefaultItemAttributes(const CT_ItemAttributeContainer::ItemAttributesVisitor& visitor) const
{
    return PS_DIAM->visitItemAttributesForType(type(), visitor);
}

bool CT_AbstractSingularItemDrawable::visitItemAttributesAdded(const CT_ItemAttributeContainer::ItemAttributesVisitor& visitor) const
{
    return m_itemAttributes.visitItemAttributes(visitor);
}

bool CT_AbstractSingularItemDrawable::visitItemAttributesInSelectedPossibilitiesOfInModel(const CT_InAbstractItemAttributeModel* inModel, const CT_ItemAttributeContainer::ItemAttributesVisitor& visitor) const
{
    Q_ASSERT(inModel != NULL);

    if(!PS_DIAM->visitItemAttributesForTypeInSelectedPossibilitiesOfInModel(type(), inModel, visitor))
        return false;

    return m_itemAttributes.visitItemAttributesOfInModel(inModel, visitor);
}

int CT_AbstractSingularItemDrawable::nItemAttributes() const
{
    return nDefaultItemAttributes() + nItemAttributesAdded();
}

int CT_AbstractSingularItemDrawable::nDefaultItemAttributes() const
{
    return PS_DIAM->nItemAttributesForType(type());
}

int CT_AbstractSingularItemDrawable::nItemAttributesAdded() const
{
    return m_itemAttributes.nItemAttributes();
}

QList<CT_AbstractItemAttribute*> CT_AbstractSingularItemDrawable::itemAttributes(const CT_InAbstractItemAttributeModel *inModel) const
{
    Q_ASSERT(inModel != NULL);

    QList<CT_AbstractItemAttribute*> l = m_itemAttributes.itemAttributesFromInModel(inModel);
    l.append(PS_DIAM->itemAttributesFromInModel(inModel, type()));

    return l;
}

QList<CT_AbstractItemAttribute *> CT_AbstractSingularItemDrawable::itemAttributes(const QList<CT_OutAbstractItemAttributeModel *>& outModelList) const
{
    QList<CT_AbstractItemAttribute *> l;

    QString myType = type();

    for(const CT_OutAbstractItemAttributeModel* outModel : outModelList) {

        CT_AbstractItemAttribute* att = outModel->isADefaultItemAttributeModel() ? PS_DIAM->itemAttributeFromOutModel(outModel, myType) : m_itemAttributes.itemAttributeFromOutModel(outModel);

        if(att != NULL)
            l.append(att);
    }

    return l;
}

QList<CT_AbstractItemAttribute *> CT_AbstractSingularItemDrawable::itemAttributes() const
{
    QList<CT_AbstractItemAttribute *> l;

    l.append(PS_DIAM->itemAttributes(type()));
    l.append(m_itemAttributes.itemAttributes());

    return l;
}

CT_AbstractItemAttribute* CT_AbstractSingularItemDrawable::firstItemAttribute(const CT_InAbstractItemAttributeModel* inModel) const
{
    Q_ASSERT(inModel != NULL);

    if(!inModel->isAtLeastOnePossibilitySelected())
        return NULL;

    CT_InStdModelPossibility* p = NULL;

    const auto visitor = [&p](const CT_InStdModelPossibility* possibility) -> bool {
        p = const_cast<CT_InStdModelPossibility*>(possibility);
        return false;
    };

    inModel->possibilitiesGroup()->visitSelectedPossibilities(visitor);

    return itemAttributeWithOutModel(static_cast<CT_OutAbstractItemAttributeModel*>(p->outModel()));
}

QList<CT_AbstractItemAttribute *> CT_AbstractSingularItemDrawable::defaultItemAttributes() const
{
    return PS_DIAM->itemAttributes(type());
}

QList<CT_AbstractItemAttribute *> CT_AbstractSingularItemDrawable::itemAttributesAdded() const
{
    return m_itemAttributes.itemAttributes();
}

void CT_AbstractSingularItemDrawable::setDefaultColor(const QColor &color)
{
    m_defaultColor = color;
}

QColor CT_AbstractSingularItemDrawable::defaultColor() const
{
    return m_defaultColor;
}

void CT_AbstractSingularItemDrawable::willBeRemovedFromResult(const CT_AbstractResult* res)
{
    m_itemAttributes.removeItemAttributeFromResult(res);
}

void CT_AbstractSingularItemDrawable::setParentGroup(const CT_StandardItemGroup* pGroup)
{
    setParentItem(pGroup);
}

CT_StandardItemGroup* CT_AbstractSingularItemDrawable::parentGroup() const
{
    return static_cast<CT_StandardItemGroup*>(parentItem());
}

CT_AbstractItem::IChildrensIteratorQtStyleSharedPtr CT_AbstractSingularItemDrawable::createQtStyleIteratorForChildrensThatUseOutModel(const CT_OutAbstractModel* outModel) const
{
    const CT_OutAbstractItemAttributeModel* iaModel = dynamic_cast<const CT_OutAbstractItemAttributeModel*>(outModel);

    if(iaModel != NULL) {
        const CT_AbstractItemAttribute* attribute = iaModel->isADefaultItemAttributeModel() ? PS_DIAM->itemAttributeFromOutModel(iaModel, type()) : m_itemAttributes.itemAttributeFromOutModel(iaModel);

        if(attribute != NULL)
            return IChildrensIteratorQtStyleSharedPtr(new ItemAttributeIterator(attribute));
    }

    return IChildrensIteratorQtStyleSharedPtr(NULL);
}
