#ifndef OPERATOREDITOR_H
#define OPERATOREDITOR_H

#include <QDialog>
#include <memory>
#include "OperatorEditorViewModel.h"

namespace Ui {
class OperatorEditor;
}

class OperatorEditor : public QDialog
{
    Q_OBJECT

public:
    explicit OperatorEditor(std::unique_ptr<OperatorEditorViewModel> vm,
                            QWidget *parent = nullptr);
    ~OperatorEditor();

private slots:
    void save();
    void cancel();
    void setOperatorIcon(int mcc, int mnc);
    void setCountryIcon(int mcc);

private:
    void init();
    std::unique_ptr<Ui::OperatorEditor> m_ui;
    std::unique_ptr<OperatorEditorViewModel> m_vm;
};

#endif // OPERATOREDITOR_H
