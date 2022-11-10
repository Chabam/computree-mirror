#include "cg_customtreeitemmodel.h"

#include <QStringList>

CG_CustomTreeItemModel::CG_CustomTreeItemModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_root = new CG_CustomTreeItem();
    m_root->setModel(this);
    m_root->setFetchMoreEnabled(false);

    m_columnCount = 0;
}

CG_CustomTreeItemModel::~CG_CustomTreeItemModel()
{
    m_root->setModel(nullptr);
    delete m_root;
}

CG_CustomTreeItem* CG_CustomTreeItemModel::invisibleRootItem() const
{
    return m_root;
}

void CG_CustomTreeItemModel::appendRow(const QList<CG_CustomTreeItem *> &items)
{
    invisibleRootItem()->appendRow(items);
}

void CG_CustomTreeItemModel::finishAppendRows()
{
    invisibleRootItem()->setFetchMoreEnabled(true);

    if(invisibleRootItem()->canFetchMore())
        invisibleRootItem()->fetchMore(indexFromItem(invisibleRootItem()));
}

bool CG_CustomTreeItemModel::hasChildren(const QModelIndex &parent) const
{
    CG_CustomTreeItem *item = itemFromIndex(parent);

    return item ? item->hasChildren() : false;
}

bool CG_CustomTreeItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;

    itemFromIndex(index)->setData(value, role);
    return true;
}

QVariant CG_CustomTreeItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    CG_CustomTreeItem *parent = static_cast<CG_CustomTreeItem*>(index.internalPointer())->parent();

    CG_CustomTreeItem *item = nullptr;

    if(parent != nullptr)
        item = parent->child(index.row(), index.column());

    if(item == nullptr)
        return QVariant();

    return item->data(role);
}

Qt::ItemFlags CG_CustomTreeItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return itemFromIndex(index)->flags();
}

QModelIndex CG_CustomTreeItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    CG_CustomTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = invisibleRootItem();
    else
        parentItem = static_cast<CG_CustomTreeItem*>(parent.internalPointer());

    CG_CustomTreeItem *childItem = parentItem->child(row);

    if (childItem)
        return createIndex(row, column, (void*)childItem);

    return QModelIndex();
}

QModelIndex CG_CustomTreeItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    CG_CustomTreeItem *childItem = static_cast<CG_CustomTreeItem*>(index.internalPointer());
    CG_CustomTreeItem *parentItem = childItem->parent();

    if (parentItem == invisibleRootItem() || (parentItem == nullptr))
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int CG_CustomTreeItemModel::rowCount(const QModelIndex &parent) const
{
    CG_CustomTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = static_cast<CG_CustomTreeItem*>(invisibleRootItem());
    else
        parentItem = static_cast<CG_CustomTreeItem*>(parent.internalPointer());

    return parentItem->rowCount();
}

int CG_CustomTreeItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_columnCount;
}

CG_CustomTreeItem *CG_CustomTreeItemModel::itemFromIndex(const QModelIndex &index) const
{
    if(!index.isValid())
        return invisibleRootItem();

    return static_cast<CG_CustomTreeItem*>(index.internalPointer());
}

QModelIndex CG_CustomTreeItemModel::indexFromItem(const CG_CustomTreeItem *item) const
{
    if(item == invisibleRootItem())
        return QModelIndex();
    else if(item)
        return createIndex(item->row(), item->column(), (void*)item);

    return QModelIndex();
}

QVariant CG_CustomTreeItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    CG_CustomTreeItem *headerItem = nullptr;

    if (orientation == Qt::Horizontal)
        headerItem = m_columnHeaderItems.at(section);

    return (headerItem != nullptr) ? headerItem->data(role) : QAbstractItemModel::headerData(section, orientation, role);
}

void CG_CustomTreeItemModel::setHorizontalHeaderLabels(const QStringList &headers)
{
    int s = headers.size();

    setColumnCount(s);

    for(int i=0; i<s; ++i)
    {
        CG_CustomTreeItem *item = horizontalHeaderItem(i);

        if(item == nullptr)
        {
            item = new CG_CustomTreeItem();
            setHorizontalHeaderItem(i, item);
        }

        item->setText(headers.at(i));
    }
}

CG_CustomTreeItem* CG_CustomTreeItemModel::horizontalHeaderItem(int column) const
{
    if ((column < 0) || (column >= columnCount()))
        return nullptr;

    return m_columnHeaderItems.at(column);
}

void CG_CustomTreeItemModel::setHorizontalHeaderItem(int column, CG_CustomTreeItem *item)
{
    if (column < 0)
        return;

    if (columnCount() <= column)
        setColumnCount(column + 1);

    CG_CustomTreeItem *oldItem = horizontalHeaderItem(column);

    if (item == oldItem)
        return;

    if (item != nullptr) {
        if (item->model() == nullptr) {
            item->setModel(this);
        }
    }

    if (oldItem != nullptr)
        oldItem->setModel(nullptr);
    delete oldItem;

    m_columnHeaderItems.replace(column, item);

    emit headerDataChanged(Qt::Horizontal, column, column);
}

void CG_CustomTreeItemModel::clear()
{
    invisibleRootItem()->clear();
}

bool CG_CustomTreeItemModel::canFetchMore(const QModelIndex &parent) const
{
    CG_CustomTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = static_cast<CG_CustomTreeItem*>(invisibleRootItem());
    else
        parentItem = static_cast<CG_CustomTreeItem*>(parent.internalPointer());

    return parentItem->canFetchMore();
}

void CG_CustomTreeItemModel::fetchMore(const QModelIndex &parent)
{
    CG_CustomTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = static_cast<CG_CustomTreeItem*>(invisibleRootItem());
    else
        parentItem = static_cast<CG_CustomTreeItem*>(parent.internalPointer());

    return parentItem->fetchMore(parent);
}

void CG_CustomTreeItemModel::setColumnCount(int c)
{
    if(m_columnHeaderItems.size() < c)
    {
        beginInsertColumns(QModelIndex(), m_columnHeaderItems.size(), c - 1);

        while(m_columnHeaderItems.size() < c)
            m_columnHeaderItems.append(nullptr);

        endInsertColumns();
    }
    else if(m_columnHeaderItems.size() > c)
    {
        beginRemoveColumns(QModelIndex(), c, m_columnHeaderItems.size() - 1);

        while(m_columnHeaderItems.size() > c)
        {
            CG_CustomTreeItem *item = m_columnHeaderItems.takeLast();
            item->setModel(nullptr);
            delete item;
        }

        endRemoveColumns();
    }

    m_columnCount = c;
}

void CG_CustomTreeItemModel::dataOfCustomItemChanged(CG_CustomTreeItem *item)
{
    emit dataChanged(indexFromItem(item), indexFromItem(item));
}
