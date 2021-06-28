#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "menu.h"
#include "customer.h"
#include <QMessageBox>
#include <QDebug> // Temp: To check what has been input

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

auto menu = loadMenu();

void MainWindow::on_RefreshMenu_clicked()
{
    // Display menu
    menu = loadMenu();
    QString menuText = displayMenu(menu);
    ui->ShowMenu->setText(menuText); // QLabel
}

void MainWindow::on_finishOrder_clicked()
{
    QString name = ui->nameInput->text();
    QString email = ui->emailInput->text();
    qDebug() << name << Qt::endl << email;
    bool emailProvided = (email.isEmpty())? false:true;
    qDebug() << emailProvided;
    ui->nameInput->clear();
    ui->emailInput->clear();
    ui->showCurrentOrder->clear();

    // auto menu = loadMenu();
    customer c(name.toStdString(), emailProvided, email.toStdString());
    c.copyOrderMap(currentOrder);
    // currentOrder.clear();

    std::string billID = c.createBill(menu);
    std::string pdfCommand = "soffice --convert-to pdf /home/ayaan/.restaurant_pos/bills/" + billID + ".txt --outdir /home/ayaan/.restaurant_pos/bills/";// + billID + ".pdf";
    std::string displayPDFCommand = "okular /home/ayaan/.restaurant_pos/bills/" + billID + ".pdf";

    system(pdfCommand.c_str());
    qDebug() << "PDF Generated";
    system(displayPDFCommand.c_str());

    // Use mutt to email to customer and then display dialog
    if (emailProvided)
    {
        QMessageBox::information(this, "Email sent", "The customer has been emailed the bill for their order.\nThank you for opting for a paperless experience.", QMessageBox::Ok);
        string emailCommand = "echo \"Thank you for dining with us\" | mutt " + email.toStdString() + " -s \"Restaurant Invoice\" -a /home/ayaan/.restaurant_pos/bills/" + billID + ".pdf";
        system(emailCommand.c_str());
    }

    // Log the sale
    for (auto item : currentOrder)
    {
        std::string item_id = to_string(item.first);
        std::string qty = to_string(item.second);
        std::string log_sale_command = "python /home/ayaan/.restaurant_pos/log_sale.py " + item_id + " " + qty;
        system(log_sale_command.c_str());
    }
    qDebug() << "Sale has been logged";

    currentOrder.clear();

}

void MainWindow::on_addItem_clicked()
{
    // auto menu = loadMenu();

    // Add that item
    QString item_id_string = ui->itemCode->text();
    int item_id = item_id_string.toInt();
    qDebug() << item_id << " with qty = ";
    int quantity = ui->Quantity->value();
    qDebug() << quantity << " added\n";

    // Store order in unordered_map currentOrder (global)
    currentOrder.insert({item_id, quantity});

    // Update current order on right
    QString updateCurrentOrder = "<table width=\"100%\"><tr><td>";
    updateCurrentOrder.append(item_id_string);
    updateCurrentOrder.append("<td>");
    updateCurrentOrder.append(menu[item_id].getName());
    updateCurrentOrder.append("<td>");
    updateCurrentOrder.append(QString::number(quantity, 10));
    updateCurrentOrder.append("</tr>");
    updateCurrentOrder.append("</table>");
    ui->showCurrentOrder->textCursor().insertHtml(updateCurrentOrder);

    ui->Quantity->setValue(1);
    ui->itemCode->clear();

    // Setup mailto
}

void MainWindow::on_cancelOrder_clicked()
{
    ui->nameInput->clear();
    ui->emailInput->clear();
    ui->showCurrentOrder->clear();
    ui->itemCode->clear();
    ui->Quantity->setValue(1);
}
