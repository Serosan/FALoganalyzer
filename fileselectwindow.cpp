#include "fileselectwindow.h"
#include "ui_fileselectwindow.h"

FileSelectWindow::FileSelectWindow(int flag,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileSelectWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    ui->tableWidget->setSelectionMode ( QAbstractItemView::SingleSelection);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setItem(0,0,new QTableWidgetItem("daEar日志"));
    ui->tableWidget->setItem(1,0,new QTableWidgetItem("faultprocess日志"));
    ui->tableWidget->setItem(2,0,new QTableWidgetItem("relay_warn日志"));
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList filelist;
    if(flag==0)
    {
       filelist=GetLogFileNameList("/Users/sero/Workspace/testdir","faultprocess*");
    }
    else if(flag==1)
    {
        filelist=GetLogFileNameList("/Users/sero/Workspace/testdir","tt*");
    }
    else
    {
        filelist=GetLogFileNameList("/Users/sero/Workspace/testdir","aa*");
    }
    ui->tableWidget->setRowCount(filelist.count());
    QStringList::iterator qit;
    int i=0;
    for(qit=filelist.begin();qit!=filelist.end();++qit)
    {
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(*qit));
        ++i;
    }
}

FileSelectWindow::~FileSelectWindow()
{
    delete ui;
}
QStringList FileSelectWindow::GetLogFileNameList(const char *path,const char *filter)
{
    QDir dir(path);
    QStringList nameFilters;
    nameFilters << filter;// << "*.png";
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    return files;
}

void FileSelectWindow::on_pushButton_2_clicked()
{
    this->hide();
}

void FileSelectWindow::on_pushButton_clicked()
{
    emit fileSelect(ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text());
}
