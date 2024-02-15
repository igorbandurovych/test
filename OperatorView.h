#ifndef OPERATORVIEW_H
#define OPERATORVIEW_H

#include <QTreeView>

class PlusButton;

class OperatorView : public QTreeView
{
    Q_OBJECT

public:
    explicit OperatorView(QWidget *parent = nullptr);

signals:
    void addClicked();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    PlusButton *m_btnAdd;
};

#endif // OPERATORVIEW_H
