#include "OperatorEditor.h"
#include "ui_OperatorEditor.h"
#include "IconStorage.h"

#include <QIcon>
#include <QIntValidator>
#include <QMessageBox>


OperatorEditor::OperatorEditor(std::unique_ptr<OperatorEditorViewModel> vm,
                               QWidget *parent):
    QDialog(parent),
    m_ui(std::make_unique<Ui::OperatorEditor>()),
    m_vm(std::move(vm))
{
    setAttribute(Qt::WA_DeleteOnClose);
    m_ui->setupUi(this);

    init();
}

OperatorEditor::~OperatorEditor() = default;

void OperatorEditor::save()
{
    int mcc = m_ui->editMcc->text().toInt();
    int mnc = m_ui->editMnc->text().toInt();
    auto name = m_ui->editName->text();
    bool success = m_vm->editMode() == OperatorEditorMode::Edit ?
                       m_vm->updateOperatorName(mcc, mnc, name):
                       m_vm->addOperator(mcc, mnc, name);

    if (!success)
    {
        QMessageBox::critical(this, tr("Error"), m_vm->lastError());
        return;
    }
    auto successMsg = m_vm->editMode() == OperatorEditorMode::Edit ?
                          tr("Successfully updated operator (%1, %2)").arg(mcc).arg(mnc) :
                          tr("Successfully added operator (%1, %2)").arg(mcc).arg(mnc);
    QMessageBox::information(this, tr("Success"), successMsg);
    accept();
}

void OperatorEditor::cancel()
{
    reject();
}

void OperatorEditor::setOperatorIcon(int mcc, int mnc)
{
    setWindowIcon(IconStorage::instance().operatorIcon(mcc, mnc));
}

void OperatorEditor::setCountryIcon(int mcc)
{
    m_ui->lblCountry->setPixmap(IconStorage::instance().countryIcon(m_vm->countryCode(mcc)));
}

static const int MaxMccMnc = 65535;

void OperatorEditor::init()
{
    if (!m_vm)
        return;

    if (m_vm->editMode() == OperatorEditorMode::Edit)
    {
        m_ui->editMcc->setReadOnly(true);
        m_ui->editMnc->setReadOnly(true);
    }
    m_ui->editName->setText(m_vm->name());
    m_ui->editMcc->setValidator(new QIntValidator{0, MaxMccMnc, this});
    m_ui->editMcc->setText(QString::number(m_vm->mcc()));
    m_ui->editMnc->setValidator(new QIntValidator{0, MaxMccMnc, this});
    m_ui->editMnc->setText(QString::number(m_vm->mnc()));
    setCountryIcon(m_vm->mcc());
    setOperatorIcon(m_vm->mcc(), m_vm->mnc());

    connect(m_ui->btnSave, &QPushButton::clicked, this, &OperatorEditor::save);
    connect(m_ui->btnCancel, &QPushButton::clicked, this, &OperatorEditor::cancel);
    connect(m_ui->editMcc, &QLineEdit::textChanged, this, [this](){
        setCountryIcon(m_ui->editMcc->text().toInt());
        setOperatorIcon(m_ui->editMcc->text().toInt(), m_ui->editMnc->text().toInt());
    });
    connect(m_ui->editMnc, &QLineEdit::textChanged, this, [this](){
        setOperatorIcon(m_ui->editMcc->text().toInt(), m_ui->editMnc->text().toInt());
    });
}
