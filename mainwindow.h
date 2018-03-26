#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include "logtypeselectwindow.h"
#include <QTableWidgetItem>
#include "fileselectwindow.h"
#include "FaultProcessAnalyser.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int ClearContent();
private:
    int initContent();
private slots:
    void on_pushButton_clicked();
    void typeSelected(int type);
    void fileSelected(QString filename);
    void ContentItemChanged(QTableWidgetItem* item);
private:
    Ui::MainWindow *ui;
    LogTypeSelectWindow *lp;
    FileSelectWindow* fp;
    FaultProcessAnalyser* processor;
    QWidget* current_content;
};

#endif // MAINWINDOW_H
