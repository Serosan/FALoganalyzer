#include "logtypeselectwindow.h"
#include "ui_logtypeselectwindow.h"

LogTypeSelectWindow::LogTypeSelectWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogTypeSelectWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setRowCount(3);
    ui->tableWidget->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    ui->tableWidget->setSelectionMode ( QAbstractItemView::SingleSelection);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setItem(0,0,new QTableWidgetItem("daEar日志"));
    ui->tableWidget->setItem(1,0,new QTableWidgetItem("faultprocess日志"));
    ui->tableWidget->setItem(2,0,new QTableWidgetItem("relay_warn日志"));
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

LogTypeSelectWindow::~LogTypeSelectWindow()
{
}

void LogTypeSelectWindow::on_pushButton_clicked()
{
    emit typeselect(ui->tableWidget->currentRow());
}
