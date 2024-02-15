#include "PlusButton.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

static const int ButtonSize = 24;

PlusButton::PlusButton(QWidget *parent):
    QWidget{parent},
    m_hovered{false},
    m_pressed{false}
{
    setFixedSize(ButtonSize, ButtonSize);
}


void PlusButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_pressed = true;
        update();
    }
}

void PlusButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_pressed)
    {
        m_pressed = false;
        update();

        if (rect().contains(event->pos()))
            emit clicked();
    }
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
void PlusButton::enterEvent(QEvent *event)
#else
void PlusButton::enterEvent(QEnterEvent *event)
#endif
{
    Q_UNUSED(event);
    m_hovered = true;
    update();
}

void PlusButton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    m_hovered = false;
    update();
}

void PlusButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    auto colorGroup = isEnabled() ? QPalette::Normal : QPalette::Disabled;
    if (colorGroup == QPalette::Normal && !isActiveWindow())
        colorGroup = QPalette::Inactive;

    auto colorRole = m_pressed ? QPalette::HighlightedText : QPalette::Text;
    painter.setPen(QPen{palette().color(colorGroup, colorRole), 2});

    if (m_pressed)
        painter.setBrush(palette().color(colorGroup, QPalette::Highlight));
    else
        painter.setBrush(Qt::NoBrush);


    if (m_hovered)
    {
        QPainterPath path;
        path.addRoundedRect(rect().adjusted(1, 1, -1, -1), 5, 5);
        painter.fillPath(path, painter.brush());
        painter.drawPath(path);
    }

    int centerX = width() / 2;
    int centerY = height() / 2;
    int plusSize = ButtonSize - 8;
    painter.drawLine(centerX - plusSize / 2, centerY, centerX + plusSize / 2, centerY);
    painter.drawLine(centerX, centerY - plusSize / 2, centerX, centerY + plusSize / 2);
}
