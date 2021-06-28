#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>
#include <unordered_map>
#include <map>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_RefreshMenu_clicked();

    void on_finishOrder_clicked();

    void on_addItem_clicked();

    void on_cancelOrder_clicked();

private:
    Ui::MainWindow *ui;
    // std::map <int, food_item> menu;
    std::unordered_map<int, int> currentOrder;
};
#endif // MAINWINDOW_H
