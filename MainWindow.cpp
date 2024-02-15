#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "OperatorModel.h"
#include "OperatorDelegate.h"
#include "OperatorEditorViewModel.h"
#include "OperatorEditor.h"

#include <QPushButton>
#include <QDebug>

MainWindow::MainWindow(const QString &dbPath, QWidget *parent):
    QMainWindow(parent),
    m_ui(std::make_unique<Ui::MainWindow>())
{
    m_ui->setupUi(this);
    auto model = new OperatorModel{dbPath};
    m_ui->view->setModel(model);
    auto delegate = new OperatorDelegate;
    m_ui->view->setItemDelegate(delegate);
    connect(delegate, &OperatorDelegate::buttonPressed, this, &MainWindow::onDelegateButtonPressed);

    
    connect(m_ui->view, &QTreeView::doubleClicked, this, [this, model](const QModelIndex &idx){
        auto type = idx.data(OperatorModelRole::ItemTypeRole).toInt();
        if (type == OperatorModelItemType::Operator)
        {
            auto name = idx.data(OperatorModelRole::NameRole).toString();
            auto mcc = idx.data(OperatorModelRole::MccRole).toInt();
            auto mnc = idx.data(OperatorModelRole::MncRole).toInt();
            auto vm = std::make_unique<OperatorEditorViewModel>(OperatorEditorMode::Edit,
                                                                name,
                                                                mcc,
                                                                mnc,
                                                                model);
            auto editor = new OperatorEditor{std::move(vm), this};
            editor->exec();
        }
    });
    
    connect(m_ui->view, &OperatorView::addClicked, this, [this, model](){
        auto vm = std::make_unique<OperatorEditorViewModel>(OperatorEditorMode::Add,
                                                            QString{},
                                                            0,
                                                            0,
                                                            model);
        auto editor = new OperatorEditor{std::move(vm), this};
        editor->exec();
    });
}

MainWindow::~MainWindow() = default;

void MainWindow::onDelegateButtonPressed(int mcc, int mnc)
{
//    Q_UNUSED(mcc);
//    Q_UNUSED(mnc);
    qDebug() << "mcc" << mcc << "mnc" << mnc;
}

