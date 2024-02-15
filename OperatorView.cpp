#include "OperatorView.h"
#include "PlusButton.h"
#include <QResizeEvent>

OperatorView::OperatorView(QWidget *parent) :
    QTreeView{parent},
    m_btnAdd{new PlusButton{this}}
{
    connect(m_btnAdd, &PlusButton::clicked, this, &OperatorView::addClicked);
    setMouseTracking(true);
}

static const int ButtonBottomMargin = 20;

void OperatorView::resizeEvent(QResizeEvent *event)
{
    QTreeView::resizeEvent(event);
    auto size = event->size();
    QPoint newPos {size.width() - m_btnAdd->width() - ButtonBottomMargin,
                   size.height() - m_btnAdd->height() - ButtonBottomMargin};
    m_btnAdd->move(newPos);
}
