/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : MichaÃ«l KREBS (ARTS/ENSAM)

 This file is part of Computree version 2.0.

 Computree is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Computree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Computree.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/


#include "dm_document.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"
#include "ct_model/outModel/abstract/ct_outabstractitemmodel.h"

#include "ct_itemdrawable/ct_standarditemgroup.h"
#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

#include "ct_result/abstract/ct_abstractresult.h"
#include "ct_model/outModel/abstract/ct_outabstractresultmodel.h"

int DM_Document::NUMBER = 1;

DM_Document::DM_Document(DM_DocumentManager &manager, QString title)
{
    _manager = &manager;
    _title = title;
    m_closeFilter = nullptr;

    m_number = NUMBER;

    ++NUMBER;

    connect(this, SIGNAL(itemDrawableToBeRemoved(CT_AbstractItemDrawable&)), this, SLOT(slotItemToBeRemoved(CT_AbstractItemDrawable&)), Qt::DirectConnection);
    connect(this, SIGNAL(itemDrawableAdded(CT_AbstractItemDrawable&)), this, SLOT(slotItemDrawableAdded(CT_AbstractItemDrawable&)), Qt::DirectConnection);
}

DM_Document::~DM_Document()
{
    QHashIterator<CT_AbstractResult*, QHash<CT_AbstractItemDrawable*, DM_AbstractInfo*>* > it(m_itemsInformation);

    while(it.hasNext())
    {
        it.next();
        qDeleteAll(it.value()->begin(), it.value()->end());
    }

    qDeleteAll(m_itemsInformation.begin(), m_itemsInformation.end());
}

void DM_Document::setDocumentCloseFilter(const DM_IDocumentCloseFilter *filter)
{
    m_closeFilter = (DM_IDocumentCloseFilter*)filter;
}

DM_IDocumentCloseFilter* DM_Document::closeFilter() const
{
    return m_closeFilter;
}

void DM_Document::addDocumentAddFilter(const DM_IDocumentAddFilter *filter)
{
    m_addFilters.append((DM_IDocumentAddFilter*)filter);
}

QList<DM_IDocumentAddFilter*> DM_Document::addFilters() const
{
    return m_addFilters;
}

DM_DocumentManager* DM_Document::getManager() const
{
    return _manager;
}

int DM_Document::getNumber() const
{
    return m_number;
}

QString DM_Document::getTitle() const
{
    return _title;
}

bool DM_Document::canAddItemDrawable(const CT_AbstractItemDrawable *item) const
{
    foreach (DM_IDocumentAddFilter* filter, m_addFilters) {
        if(!filter->canAddItemDrawable(this, item))
            return false;
    }

    return true;
}

void DM_Document::addItemDrawable(CT_AbstractItemDrawable &item)
{
    if(!canAddItemDrawable(&item))
        return;

    if(item.addDocumentParent(this))
    {
        _listItemDrawable.append((CT_AbstractItemDrawable*)&item);

        emit itemDrawableAdded(item);
    }
}

void DM_Document::removeItemDrawable(CT_AbstractItemDrawable &item)
{
    if(_listItemDrawable.contains(&item))
    {
        emit itemDrawableToBeRemoved(item);

        item.removeDocumentParent(this);

        _listItemDrawable.removeOne(&item);
    }
}

void DM_Document::removeAllItemDrawableOfResult(const CT_AbstractResult &res)
{
    // MK : 05.12.18 original code
    /*CT_AbstractResult *oResult = (CT_AbstractResult*)&res;
    CT_AbstractResult *result = (CT_AbstractResult*)&res;

    if(res.model() != nullptr)
    {
        CT_OutAbstractResultModel *om = (CT_OutAbstractResultModel*)res.model()->recursiveOriginalModel();

        if((om != nullptr)
            && !om->childrens().isEmpty())
            oResult = ((CT_OutAbstractModel*)om->childrens().first())->result();
    }

    beginRemoveMultipleItemDrawable();

    QMutableListIterator<CT_AbstractItemDrawable*> it(_listItemDrawable);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();
        CT_AbstractResult *iResult = item->result();

        if((iResult == result) || (iResult == oResult))
        {
            emit itemDrawableToBeRemoved(*item);

            item->removeDocumentParent(this);
            it.remove();
        }
    }

    endRemoveMultipleItemDrawable();

    redrawGraphics();*/

    // MK 05.12.18 new code (to test it!)
    const CT_AbstractResult* currentResult = &res;
    const CT_AbstractResult* originalResult = static_cast<CT_AbstractResult*>(currentResult->model()->recursiveOriginalModel()->result());

    beginRemoveMultipleItemDrawable();

    QMutableListIterator<CT_AbstractItemDrawable*> it(_listItemDrawable);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable* item = it.next();
        const CT_AbstractResult* resultOfItem = item->result();

        if((resultOfItem == currentResult)
                || (resultOfItem == originalResult))
        {
            emit itemDrawableToBeRemoved(*item);

            item->removeDocumentParent(this);
            it.remove();
        }
    }

    endRemoveMultipleItemDrawable();

    redrawGraphics();
}

void DM_Document::removeAllItemDrawableOfModel(const CT_OutAbstractModel &model)
{
    const CT_OutAbstractModel* lom = model.recursiveOriginalModelWithAResult();

    beginRemoveMultipleItemDrawable();

    QMutableListIterator<CT_AbstractItemDrawable*> it(_listItemDrawable);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        if((item->model() == lom) || (item->model() == &model))
        {
            emit itemDrawableToBeRemoved(*item);

            item->removeDocumentParent(this);
            it.remove();
        }
    }

    endRemoveMultipleItemDrawable();

    redrawGraphics();
}

void DM_Document::removeAllSelectedItemDrawable()
{
    beginRemoveMultipleItemDrawable();

    QMutableListIterator<CT_AbstractItemDrawable*> it(_listItemDrawable);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        if(item->isSelected())
        {
            emit itemDrawableToBeRemoved(*item);

            item->removeDocumentParent(this);
            it.remove();
        }
    }

    endRemoveMultipleItemDrawable();

    redrawGraphics();
}

void DM_Document::removeAllItemDrawable()
{
    beginRemoveMultipleItemDrawable();

    QMutableListIterator<CT_AbstractItemDrawable*> it(_listItemDrawable);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        emit itemDrawableToBeRemoved(*item);

        item->removeDocumentParent(this);
        it.remove();
    }

    endRemoveMultipleItemDrawable();

    redrawGraphics();
}

const QList<CT_AbstractItemDrawable*>& DM_Document::getItemDrawable() const
{
    return _listItemDrawable;
}

const QHash<CT_AbstractResult *, QHash<CT_AbstractItemDrawable *, DM_AbstractInfo *> *>& DM_Document::getItemsInformations() const
{
    return m_itemsInformation;
}

QList<CT_AbstractItemDrawable*> DM_Document::getSelectedItemDrawable() const
{
    QList<CT_AbstractItemDrawable*> list;

    QListIterator<CT_AbstractItemDrawable*> it(_listItemDrawable);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        if(item->isSelected())
            list.append(item);
    }

    return list;
}

bool DM_Document::containsItemDrawable(const CT_AbstractItemDrawable* item) const
{
    return item->isInDocument(this);
}

bool DM_Document::containsItemDrawableOrAtLeastOneChildren(const CT_AbstractItemDrawable* item) const
{
    CT_OutAbstractModel* model = item->model();

    if(model->isVisibleInDocument(this))
        return true;

    const DM_Document* doc = this;

    return !model->recursiveVisitOutChildrens([&doc](const CT_OutAbstractModel* child) -> bool {
        return !child->isVisibleInDocument(doc);
    });
}

bool DM_Document::useItemColor() const
{
    return false;
}

void DM_Document::setColor(const CT_AbstractItemDrawable *item, const QColor &color)
{
    Q_UNUSED(item)
    Q_UNUSED(color)
}

bool DM_Document::isColorModified(const CT_AbstractItemDrawable *item)
{
    Q_UNUSED(item)

    return false;
}

QColor DM_Document::getColor(const CT_AbstractItemDrawable *item)
{
    Q_UNUSED(item)

    return Qt::white;
}

bool DM_Document::canChangeVisibility() const
{
    return false;
}

void DM_Document::setVisible(const CT_AbstractItemDrawable *item, bool visible)
{
    Q_UNUSED(item)
    Q_UNUSED(visible)
}

bool DM_Document::isVisible(const CT_AbstractItemDrawable *item) const
{
    Q_UNUSED(item)

    return false;
}

size_t DM_Document::nItemDrawable() const
{
    return _listItemDrawable.size();
}

CT_AbstractItemDrawable* DM_Document::getItemDrawable(int i) const
{
    if((i>=0)
        && (i<_listItemDrawable.size()))
    {
        return _listItemDrawable.at(i);
    }

    return nullptr;
}

void DM_Document::setSelectAllItemDrawable(bool select)
{
    QListIterator<CT_AbstractItemDrawable*> it(_listItemDrawable);

    while(it.hasNext())
        it.next()->setSelected(select);
}

void DM_Document::setSelectAllItemDrawableOfModel(bool select, const CT_OutAbstractModel &model)
{
    const CT_OutAbstractModel* lom = model.recursiveOriginalModelWithAResult();

    QListIterator<CT_AbstractItemDrawable*> it(_listItemDrawable);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        if((item->model() == lom) || (item->model() == &model))
            item->setSelected(select);
    }
}

QList<CT_AbstractItemDrawable*> DM_Document::findItemDrawable(const CT_OutAbstractModel &model) const
{
    const CT_OutAbstractModel* lom = model.recursiveOriginalModelWithAResult();

    QList<CT_AbstractItemDrawable*> list;

    QListIterator<CT_AbstractItemDrawable*> it(_listItemDrawable);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        if((item->model() == lom) || (item->model() == &model))
            list.append(item);
    }

    return list;
}

void DM_Document::findItemDrawable(const CT_OutAbstractModel &model, QList<CT_AbstractItemDrawable *> &outList) const
{
    const CT_OutAbstractModel* lom = model.recursiveOriginalModelWithAResult();

    outList.clear();

    QListIterator<CT_AbstractItemDrawable*> it(_listItemDrawable);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        if((item->model() == lom) || (item->model() == &model))
            outList.append(item);
    }
}

CT_AbstractItemDrawable* DM_Document::findFirstItemDrawable(const CT_OutAbstractModel &model) const
{
    const CT_OutAbstractModel* lom = model.recursiveOriginalModelWithAResult();

    QListIterator<CT_AbstractItemDrawable*> it(_listItemDrawable);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        if((item->model() == lom) || (item->model() == &model))
            return item;
    }

    return nullptr;
}

QHash<CT_AbstractItemDrawable*, DM_AbstractInfo*>* DM_Document::createItemInformationsForResult(CT_AbstractResult *result)
{
    if(result == nullptr)
        return nullptr;

    QHash<CT_AbstractItemDrawable*, DM_AbstractInfo*> *hash = new QHash<CT_AbstractItemDrawable*, DM_AbstractInfo*>();
    m_itemsInformation.insert(result, hash);

    connect(result, SIGNAL(destroyed(QObject*)), this, SLOT(slotResultDestroyed(QObject*)), Qt::DirectConnection);

    return hash;
}

DM_AbstractInfo* DM_Document::createNewItemInformation(const CT_AbstractItemDrawable *item) const
{
    Q_UNUSED(item)

    return nullptr;
}

void DM_Document::slotItemDrawableAdded(CT_AbstractItemDrawable &item)
{
    connect(&item, SIGNAL(selectChange(bool)), this, SLOT(slotItemDrawableSelectionChanged(bool)), Qt::QueuedConnection);

    if(item.result() == nullptr)
        return;

    QHash<CT_AbstractItemDrawable*, DM_AbstractInfo*>* hash = m_itemsInformation.value(item.result(), nullptr);

    if(hash == nullptr)
    {
        hash = new QHash<CT_AbstractItemDrawable*, DM_AbstractInfo*>();
        m_itemsInformation.insert(item.result(), hash);

        connect(item.result(), SIGNAL(destroyed(QObject*)), this, SLOT(slotResultDestroyed(QObject*)), Qt::DirectConnection);

        DM_AbstractInfo* info = createNewItemInformation(&item);

        if(info == nullptr)
            return;

        hash->insert(&item, info);
    }

    CT_StandardItemGroup* group = dynamic_cast<CT_StandardItemGroup*>(&item);

    if(group != nullptr) {
        group->recursiveVisitChildrensOfTypeItem([this, &hash](const CT_AbstractItemDrawable* child) -> bool {

            if(!hash->contains(const_cast<CT_AbstractItemDrawable*>(child)))
            {
                DM_AbstractInfo* childInfo = this->createNewItemInformation(child);

                if(childInfo != nullptr)
                    hash->insert(const_cast<CT_AbstractItemDrawable*>(child), childInfo);
            }

            return true;
        });
    }
}

void DM_Document::slotItemToBeRemoved(CT_AbstractItemDrawable &item)
{
    disconnect(&item, SIGNAL(selectChange(bool)), this, SLOT(slotItemDrawableSelectionChanged(bool)));
}

/////////// PRIVATE ////////////

void DM_Document::slotItemDrawableSelectionChanged(bool select)
{
    CT_AbstractItemDrawable *item = (CT_AbstractItemDrawable*)sender();

    emit itemDrawableSelectionChanged(item, select);
}

void DM_Document::slotResultDestroyed(QObject *result)
{
    QHash<CT_AbstractItemDrawable*, DM_AbstractInfo*> *hash = m_itemsInformation.take((CT_AbstractResult*)result);

    if(hash != nullptr)
        qDeleteAll(hash->begin(), hash->end());

    delete hash;
}
