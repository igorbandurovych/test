#ifndef PLUSBUTTON_H
#define PLUSBUTTON_H

#include <QWidget>

class PlusButton : public QWidget
{
    Q_OBJECT

public:
    explicit PlusButton(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    void enterEvent(QEvent *event) override;
#else
    void enterEvent(QEnterEvent *event) override;
#endif
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

signals:
    void clicked();

private:
    bool m_hovered;
    bool m_pressed;
};

#endif // PLUSBUTTON_H
