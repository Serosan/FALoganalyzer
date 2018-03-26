#ifndef FILESELECTWINDOW_H
#define FILESELECTWINDOW_H

#include <QDialog>
#include <QDir>
namespace Ui {
class FileSelectWindow;
}

class FileSelectWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FileSelectWindow(int flag,QWidget *parent = 0);
    ~FileSelectWindow();
    QStringList GetLogFileNameList(const char *path,const char *filter);
signals:
    void fileSelect(QString filename);
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::FileSelectWindow *ui;
};

#endif // FILESELECTWINDOW_H
