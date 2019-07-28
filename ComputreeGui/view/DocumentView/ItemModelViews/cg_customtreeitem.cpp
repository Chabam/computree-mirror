#include "cg_customtreeitem.h"

#include "view/DocumentView/ItemModelViews/cg_customtreeitemmodel.h"
#include "dm_guimanager.h"

CG_CustomTreeItem::CG_CustomTreeItem()
{
    m_count = 0;
    m_parent = nullptr;
    m_column = 0;
    m_model = nullptr;
    m_fetchSize = 30;
    m_fetchMoreEnabled = true;

    setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable
             |Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled);
}

CG_CustomTreeItem::CG_CustomTreeItem(const QString &text)
{
    m_count = 0;
    m_parent = nullptr;
    m_column = 0;
    m_model = nullptr;
    m_fetchSize = 30;
    m_fetchMoreEnabled = true;

    setText(text);
    setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable
             |Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled);
}

CG_CustomTreeItem::~CG_CustomTreeItem()
{
    clear();
}

void CG_CustomTreeItem::appendRow(const QList<CG_CustomTreeItem *> &items)
{
    m_items.append(items);

    int c = 0;

    foreach (CG_CustomTreeItem *i, items) {
        i->setParent(this);
        i->setColumn(c);
        i->setModel(model());
        ++c;
    }
}

void CG_CustomTreeItem::removeRow(int r)
{
    bool send = (m_count > r);

    if((model() != nullptr) && send)
        model()->beginRemoveRows(model()->indexFromItem(this), r, r);

    QList<CG_CustomTreeItem *> items = m_items.takeAt(r);

    foreach (CG_CustomTreeItem *i, items) {
        i->setModel(nullptr);
        delete i;
    }

    if(send)
        --m_count;

    if((model() != nullptr) && send)
        model()->endRemoveRows();
}

void CG_CustomTreeItem::insertRow(int i, const QList<CG_CustomTreeItem *> &items)
{
    bool send = (m_count > i);

    if((model() != nullptr) && send)
        model()->beginInsertRows(model()->indexFromItem(this), i, i);

    int c = 0;

    foreach (CG_CustomTreeItem *i, items) {
        i->setParent(this);
        i->setColumn(c);
        i->setModel(model());
        ++c;
    }

    m_items.insert(i, items);

    if(send)
        ++m_count;

    if((model() != nullptr) && send)
        model()->endInsertRows();
}

void CG_CustomTreeItem::replaceRow(int r, const QList<CG_CustomTreeItem *> &items)
{
    if(model() != nullptr)
        model()->beginResetModel();

    QList<CG_CustomTreeItem *> itemsT = m_items.takeAt(r);

    foreach (CG_CustomTreeItem *i, itemsT) {
        i->setModel(nullptr);
        delete i;
    }

    m_items.insert(r, items);

    int c = 0;

    foreach (CG_CustomTreeItem *i, items) {
        i->setParent(this);
        i->setColumn(c);
        i->setModel(model());
        ++c;
    }

    if(model() != nullptr)
        model()->endResetModel();
}

bool CG_CustomTreeItem::hasChildren() const
{
    return !m_items.isEmpty();
}

int CG_CustomTreeItem::row() const
{
    if(parent() == nullptr)
        return 0;

    int row = 0;

    foreach (const QList<CG_CustomTreeItem*> l, parent()->m_items) {
        if(l.at(column()) == this)
            return row;

        ++row;
    }

    return 0;
}

int CG_CustomTreeItem::column() const
{
    return m_column;
}

CG_CustomTreeItem* CG_CustomTreeItem::parent() const
{
    return m_parent;
}

CG_CustomTreeItem* CG_CustomTreeItem::child(int row, int column)
{
    return m_items.at(row).at(column);
}

QVariant CG_CustomTreeItem::data(int role) const
{
    role = (role == Qt::EditRole) ? Qt::DisplayRole : role;

    return m_datas.value(role, QVariant());
}

void CG_CustomTreeItem::setData(const QVariant &value, int role)
{
    role = (role == Qt::EditRole) ? Qt::DisplayRole : role;

    setDataWithoutSignal(value, role);

    emit dataChanged(this, role, value);

    if(role == Qt::CheckStateRole)
        emit checkStateChanged(checkState());
}

void CG_CustomTreeItem::setText(const QString &text)
{
    setDataWithoutSignal(text, Qt::DisplayRole);
}

QString CG_CustomTreeItem::text() const
{
    return data(Qt::DisplayRole).toString();
}

void CG_CustomTreeItem::setFlags(Qt::ItemFlags flags)
{
    setDataWithoutSignal((int)flags, Qt::UserRole - 1);
}

Qt::ItemFlags CG_CustomTreeItem::flags() const
{
    QVariant v = data(Qt::UserRole - 1);

    if (!v.isValid())
        return (Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable
                |Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled);

    return Qt::ItemFlags(v.toInt());
}

void CG_CustomTreeItem::setSelectable(bool e)
{
    changeFlags(e, Qt::ItemIsSelectable);
}

void CG_CustomTreeItem::setCheckable(bool e)
{
    if (e && !isCheckable()) {
        // make sure there's data for the checkstate role
        if (!data(Qt::CheckStateRole).isValid())
            setDataWithoutSignal(Qt::Unchecked, Qt::CheckStateRole);
    }

    changeFlags(e, Qt::ItemIsUserCheckable);
}

void CG_CustomTreeItem::setTristate(bool e)
{
    changeFlags(e, Qt::ItemIsTristate);
}

void CG_CustomTreeItem::setDragEnabled(bool e)
{
    changeFlags(e, Qt::ItemIsDragEnabled);
}

void CG_CustomTreeItem::setDropEnabled(bool e)
{
    changeFlags(e, Qt::ItemIsDropEnabled);
}

void CG_CustomTreeItem::setEditable(bool e)
{
    changeFlags(e, Qt::ItemIsEditable);
}

void CG_CustomTreeItem::setEnabled(bool e)
{
    changeFlags(e, Qt::ItemIsEnabled);
}

bool CG_CustomTreeItem::isCheckable() const
{
    return (flags() & Qt::ItemIsUserCheckable) != 0;
}

void CG_CustomTreeItem::setCheckState(Qt::CheckState c)
{
    setDataWithoutSignal(c, Qt::CheckStateRole);
}

Qt::CheckState CG_CustomTreeItem::checkState() const
{
    return Qt::CheckState(qvariant_cast<int>(data(Qt::CheckStateRole)));
}

void CG_CustomTreeItem::setFetchSize(int n)
{
    if(n > 0)
        m_fetchSize = n;
}

int CG_CustomTreeItem::fetchSize() const
{
    return m_fetchSize;
}

int CG_CustomTreeItem::rowCount() const
{
    return m_count;
}

int CG_CustomTreeItem::nChildrens() const
{
    return m_items.size();
}

bool CG_CustomTreeItem::canFetchMore() const
{
    return m_fetchMoreEnabled ? m_count < m_items.size() : false;
}

void CG_CustomTreeItem::fetchMore(const QModelIndex &parentIndex)
{
    int reminder = m_items.size() - m_count;
    int itemsToFetch = reminder;

    if(parent() == nullptr)
        itemsToFetch = qMin(m_fetchSize, reminder);

    if(model() != nullptr)
        model()->beginInsertRows(parentIndex, m_count, m_count + itemsToFetch - 1);

    m_count += itemsToFetch;

    if(model() != nullptr)
        model()->endInsertRows();
}

void CG_CustomTreeItem::setFetchMoreEnabled(bool e)
{
    m_fetchMoreEnabled = e;
}

void CG_CustomTreeItem::clear()
{
    if(parent() == nullptr)
        setFetchMoreEnabled(false);

    if(!m_items.isEmpty())
    {
        int n = rowCount();

        if((model() != nullptr) && (n > 0))
            model()->beginRemoveRows(model()->indexFromItem(this), 0, n-1);

        foreach (const QList<CG_CustomTreeItem*> &l, m_items) {
            qDeleteAll(l.begin(), l.end());
        }

        m_items.clear();
        m_count = 0;

        if((model() != nullptr) && (n > 0))
            model()->endRemoveRows();
    }
}

void CG_CustomTreeItem::setBoolData(bool e, Qt::ItemDataRole role)
{
    setDataWithoutSignal(e ? Qt::Checked : Qt::Unchecked, role);
}

void CG_CustomTreeItem::changeFlags(bool enable, Qt::ItemFlags f)
{
    Qt::ItemFlags fl = flags();

    if (enable)
        fl |= f;
    else
        fl &= ~f;

    setFlags(fl);
}

void CG_CustomTreeItem::setDataWithoutSignal(const QVariant &value, int role)
{
    role = (role == Qt::EditRole) ? Qt::DisplayRole : role;

    m_datas.insert(role, value);

    if(model() != nullptr)
        model()->dataOfCustomItemChanged(this);
}

void CG_CustomTreeItem::setParent(const CG_CustomTreeItem *p)
{
    m_parent = (CG_CustomTreeItem*)p;
}

void CG_CustomTreeItem::setColumn(int c)
{
    m_column = c;
}

void CG_CustomTreeItem::setModel(const CG_CustomTreeItemModel *m)
{
    m_model = (CG_CustomTreeItemModel*)m;

    QListIterator< QList<CG_CustomTreeItem *> > it(m_items);

    while(it.hasNext())
    {
        QListIterator<CG_CustomTreeItem *> it2(it.next());

        while(it2.hasNext())
            it2.next()->setModel(m);
    }
}

CG_CustomTreeItemModel* CG_CustomTreeItem::model() const
{
    return m_model;
}
