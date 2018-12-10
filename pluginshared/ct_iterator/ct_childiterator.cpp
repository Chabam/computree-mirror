#include "ct_childiterator.h"

#include "ct_item/abstract/ct_abstractitem.h"

CT_ChildIterator::CT_ChildIterator(const CT_AbstractItem *parent)
{
    // TODO : MK 19.11.18
    //m_childrens = parent->childrensForGui();
    m_currentPos = -1;
}

CT_ChildIterator::~CT_ChildIterator()
{
}

bool CT_ChildIterator::hasNext() const
{
    return ((m_currentPos+1) < m_childrens.size());
}

const CT_AbstractItem* CT_ChildIterator::next()
{
    return m_childrens[++m_currentPos];
}

const CT_AbstractItem* CT_ChildIterator::current() const
{
    return m_childrens[m_currentPos];
}
