#ifndef LOGTYPESELECTWINDOW_H
#define LOGTYPESELECTWINDOW_H

#include <QDialog>
#include <fileselectwindow.h>
#include <QDir>
namespace Ui {
class LogTypeSelectWindow;
}

class LogTypeSelectWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LogTypeSelectWindow(QWidget *parent = 0);
    ~LogTypeSelectWindow();
signals:
    void typeselect(int type);
private slots:
    void on_pushButton_clicked();

private:
    Ui::LogTypeSelectWindow *ui;
};

#endif // LOGTYPESELECTWINDOW_H
