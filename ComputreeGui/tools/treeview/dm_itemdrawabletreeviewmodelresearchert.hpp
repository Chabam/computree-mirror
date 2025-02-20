#include "tools/treeview/dm_itemdrawabletreeviewmodelresearchert.h"

template<class Item>
DM_ItemDrawableTreeViewModelResearcherT<Item>::DM_ItemDrawableTreeViewModelResearcherT() : DM_AbstractWorker()
{
    m_collection = nullptr;
    m_treeViewManager = nullptr;
}

template<class Item>
void DM_ItemDrawableTreeViewModelResearcherT<Item>::setCollection(const QVector<QList<Item*> > *collection)
{
    m_collection = const_cast<QVector<QList<Item*> >*>(collection);
}

template<class Item>
void DM_ItemDrawableTreeViewModelResearcherT<Item>::setTreeViewManager(const DM_ITreeViewManagerT<Item> *man)
{
    m_treeViewManager = const_cast<DM_ITreeViewManagerT<Item>*>(man);
}

template<class Item>
void DM_ItemDrawableTreeViewModelResearcherT<Item>::setItemDrawable(const QList<CT_AbstractItemDrawable *> &list)
{
    m_items = list;
}

template<class Item>
void DM_ItemDrawableTreeViewModelResearcherT<Item>::apply()
{
    int size = m_items.size();
    int i = 0;

    m_collection->resize(size);

    QListIterator<CT_AbstractItemDrawable *> it(m_items);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        Item* sItem = m_treeViewManager->itemFromItemDrawable(item);

        if(sItem != nullptr)
            (*m_collection)[i].append(sItem);

        ++i;
        setProgress((i*100)/size);
    }

    setFinished();
}
