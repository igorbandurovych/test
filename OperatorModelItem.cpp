#include "OperatorModelItem.h"
#include "Constants.h"


OperatorModelItem::OperatorModelItem(const QVariantList &data, OperatorModelItem *parent) :
    m_data(data),
    m_parent(parent)
{
}

OperatorModelItem::~OperatorModelItem()
{
    qDeleteAll(m_children);
}

OperatorModelItem *OperatorModelItem::childAt(int index)
{
    return index < 0 || index >= m_children.size() ? nullptr : m_children.at(index);
}

int OperatorModelItem::childCount() const
{
    return m_children.count();
}

void OperatorModelItem::appendChild(const QVariantList &data)
{
    m_children.append(new OperatorModelItem{data, this});
}

void OperatorModelItem::appendChild(OperatorModelItem *child)
{
    m_children.append(child);
}

const QVariantList &OperatorModelItem::data() const
{
    return m_data;
}

bool OperatorModelItem::insertChildren(int pos, int count)
{
    if (pos < 0 || pos > m_children.size())
        return false;

    for (int i = 0; i < count; ++i)
        m_children.insert(pos, new OperatorModelItem{QVariantList{}, this});

    return true;
}

OperatorModelItem *OperatorModelItem::parent()
{
    return m_parent;
}

bool OperatorModelItem::removeChildren(int pos, int count)
{
    if (pos < 0 || pos + count > m_children.size())
        return false;

    for (int i = 0; i < count; ++i)
        delete m_children.takeAt(pos);

    return true;
}

int OperatorModelItem::childNumber() const
{
    return m_parent ? m_parent->m_children.indexOf(const_cast<OperatorModelItem *>(this)) : 0;
}

bool OperatorModelItem::setData(const QVariantList &val)
{
    m_data = val;
    return true;
}

int OperatorModelItem::type() const
{
    if (!m_parent)
        return OperatorModelItemType::Root;

    if (!m_parent->m_parent)
        return OperatorModelItemType::Country;

    return OperatorModelItemType::Operator;
}
