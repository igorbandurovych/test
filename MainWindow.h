#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString &dbPath, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDelegateButtonPressed(int mcc, int mnc);

private:
    std::unique_ptr<Ui::MainWindow> m_ui;
};
#endif // MAINWINDOW_H
