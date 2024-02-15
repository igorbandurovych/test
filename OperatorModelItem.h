#ifndef OPERATORMODELITEM_H
#define OPERATORMODELITEM_H

#include <QVariantList>
#include <QVector>

class OperatorModelItem
{
public:
    explicit OperatorModelItem(const QVariantList &data = {},
                               OperatorModelItem *parent = nullptr);
    ~OperatorModelItem();

    OperatorModelItem *childAt(int index);
    int childCount() const;
    void appendChild(const QVariantList &data);
    void appendChild(OperatorModelItem *child);
    bool insertChildren(int pos, int count);
    bool removeChildren(int pos, int count);
    const QVariantList &data() const;
    bool setData(const QVariantList &val);
    int type() const;
    OperatorModelItem *parent();
    int childNumber() const;
private:
    QVector<OperatorModelItem *> m_children;
    QVariantList m_data;
    OperatorModelItem *m_parent;
};

#endif // OPERATORMODELITEM_H
