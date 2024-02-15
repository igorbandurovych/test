#include "OperatorDelegate.h"
#include "Constants.h"

#include <QApplication>
#include <QPainter>
#include <QPainterPath>

#include <QDebug>
#include <QMouseEvent>

OperatorDelegate::OperatorDelegate(QObject *parent) :
    QStyledItemDelegate{parent}
{
}

static const int ButtonMargin = 3;

static QRect getButtonRect(const QStyleOptionViewItem &opt, const QModelIndex &index)
{
    auto style = opt.widget ? opt.widget->style() : QApplication::style();
    auto textRect = style->proxy()->subElementRect(QStyle::SE_ItemViewItemText,
                                                   &opt,
                                                   opt.widget);
    QFontMetrics fontMetrics{opt.font};
    auto finalText = fontMetrics.elidedText(index.data().toString(), opt.textElideMode, textRect.width());
    int textWidth = fontMetrics.horizontalAdvance(finalText) +
                    style->proxy()->pixelMetric(QStyle::PM_FocusFrameHMargin, &opt, opt.widget) + 1;
    int btnSide = textRect.height();
    int left = textRect.left() + textWidth + 2 * ButtonMargin + btnSide > opt.rect.right() ?
        opt.rect.right() - 2 * ButtonMargin - btnSide :
        textRect.left() + textWidth + ButtonMargin;
    return {left, textRect.top(), btnSide, btnSide};
}

void OperatorDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_ASSERT(index.isValid());

    auto opt = option;
    initStyleOption(&opt, index);

    auto style = opt.widget ? opt.widget->style() : QApplication::style();

    auto itemType = index.data(OperatorModelRole::ItemTypeRole).toInt();

    if (itemType == OperatorModelItemType::Country)
        opt.font.setWeight(QFont::Bold); // for Qt::DisplayRole

    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

    if (itemType == OperatorModelItemType::Operator && index == m_hoveredIndex)
    {
        painter->save();

        painter->setRenderHint(QPainter::Antialiasing);
        auto btnRect = getButtonRect(opt, index);
        auto colorGroup = opt.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
        if (colorGroup == QPalette::Normal && !(opt.state & QStyle::State_Active))
            colorGroup = QPalette::Inactive;
        auto colorRole = m_buttonPressStates.value(index, false) ?
                             QPalette::HighlightedText :
                             QPalette::Text;
        painter->setPen(QPen{opt.palette.color(colorGroup, colorRole)});

        if (m_buttonHoverStates.value(index, false))
        {
            QBrush brush {Qt::NoBrush};
            if (m_buttonPressStates.value(index, false))
                brush = opt.palette.color(colorGroup, QPalette::Highlight);
            QPainterPath path;
            path.addRoundedRect(btnRect.adjusted(1, 1, -1, -1), 3, 3);
            painter->fillPath(path, brush);
            painter->drawPath(path);
        }

        int centerX = btnRect.left() + btnRect.width() / 2;
        int centerY = btnRect.top() + btnRect.height() / 2;
        int plusSize = btnRect.height() - 8;
        painter->drawLine(centerX - plusSize / 2, centerY, centerX + plusSize / 2, centerY);
        painter->drawLine(centerX, centerY - plusSize / 2, centerX, centerY + plusSize / 2);

        painter->restore();
    }
}

bool OperatorDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_ASSERT(event && model);
    auto opt = option;
    initStyleOption(&opt, index);

    if (event->type() == QEvent::MouseMove && m_hoveredIndex != index)
    {
        if (m_buttonHoverStates.value(m_hoveredIndex, false))
            m_buttonHoverStates[m_hoveredIndex] = false;

        if (m_buttonPressStates.value(m_hoveredIndex, false))
            m_buttonPressStates[m_hoveredIndex] = false;

        m_hoveredIndex = index;
    }

    if (index.data(OperatorModelRole::ItemTypeRole).toInt() == OperatorModelItemType::Operator)
    {
        switch (event->type())
        {
            case QEvent::MouseMove:
            {
                auto mouseEvent = static_cast<QMouseEvent *>(event);
                bool isHovered = getButtonRect(opt, index).contains(mouseEvent->pos());
                if (m_buttonHoverStates.value(index, false) != isHovered)
                {
                    m_buttonHoverStates[index] = isHovered;
                    emit sizeHintChanged(index);
                }
                break;
            }
            case QEvent::MouseButtonPress:
            {
                auto mouseEvent = static_cast<QMouseEvent *>(event);
                if (getButtonRect(opt, index).contains(mouseEvent->pos()))
                {
                    m_buttonPressStates[index] = true;
                    emit sizeHintChanged(index);
                }
                break;
            }
            case QEvent::MouseButtonRelease:
            {
                if (m_buttonPressStates.value(index, false))
                {
                    m_buttonPressStates[index] = false;
                    emit sizeHintChanged(index);
                    if (getButtonRect(opt, index).contains(static_cast<QMouseEvent *>(event)->pos()))
                    {
                        int mcc = index.data(OperatorModelRole::MccRole).toInt();
                        int mnc = index.data(OperatorModelRole::MncRole).toInt();
                        emit buttonPressed(mcc, mnc);
                    }
                }
                break;
            }
            default:
                break;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
