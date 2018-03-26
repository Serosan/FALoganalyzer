#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QTextEdit>
#include "FaultProcessAnalyser.h"
#include <iostream>
MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    lp=nullptr;
    fp=nullptr;
    ui->setupUi(this);

    ui->tableWidget->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    ui->tableWidget->setSelectionMode ( QAbstractItemView::SingleSelection);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(ContentItemChanged(QTableWidgetItem*)));
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(lp!=nullptr)
    {
        delete lp;
        lp=nullptr;
    }
    if(fp!=nullptr)
    {
        delete fp;
        fp=nullptr;
    }
    if(processor!=nullptr)
    {
        delete processor;
        processor=nullptr;
    }
}
void MainWindow::typeSelected(int type)
{
    fp=new FileSelectWindow(type);
    if(lp!=nullptr)
    {
        delete lp;
        lp=nullptr;
    }
    connect(fp,SIGNAL(fileSelect(QString)),this,SLOT(fileSelected(QString)));
    if(processor!=nullptr)
    {
        delete processor;
    }
    processor=new FaultProcessAnalyser();
    fp->show();
}
void MainWindow::on_pushButton_clicked()
{
    lp=new LogTypeSelectWindow();
    connect(lp,SIGNAL(typeselect(int)),this,SLOT(typeSelected(int)));
    lp->show();
}
void MainWindow::fileSelected(QString filename)
{
    ClearContent();
    if(fp!=nullptr)
    {
        delete fp;
        fp=nullptr;
    }
    QString fullfile=QString("/Users/sero/Workspace/testdir/")+filename;
    processor->StartAnalysis(fullfile.toStdString().c_str());
    processor->SetTableInfo(ui->tableWidget);
    current_content=processor->InitContentWidget(this);
    ui->gridLayout->addWidget(current_content);

}
void MainWindow::ContentItemChanged(QTableWidgetItem *item)
{
    int row_nbr=item->row();
    processor->ChangePageInfo(ui->tableWidget->itemAt(row_nbr,0)->text().toInt()-1);
    ui->gridLayout->removeWidget(current_content);
    delete current_content;
    current_content=processor->InitContentWidget(this);
    ui->gridLayout->addWidget(current_content);
}

int MainWindow::ClearContent()
{
    if(current_content!=nullptr)
    {
        ui->gridLayout->removeWidget(current_content);
        delete current_content;
        current_content=nullptr;
    }
    ui->tableWidget->clearContents();
    return 1;
}
