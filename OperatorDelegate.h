#ifndef OPERATORDELEGATE_H
#define OPERATORDELEGATE_H

#include <QStyledItemDelegate>

class OperatorDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    OperatorDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:
    void buttonPressed(int mcc, int mnc);

private:
    Q_DISABLE_COPY(OperatorDelegate);

    QModelIndex m_hoveredIndex;
    QMap<QModelIndex, bool> m_buttonHoverStates;
    QMap<QModelIndex, bool> m_buttonPressStates;
};

#endif // OPERATORDELEGATE_H
