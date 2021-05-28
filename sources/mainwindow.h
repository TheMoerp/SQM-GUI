#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sqmtablemodel.h"

class SQMTableModel;
class QSpinBox;
class QTableView;

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
    void SetModel();

private:
    Ui::MainWindow *ui;
    QSpinBox *spinBase, *spinExp, *spinMod;
    QTableView *resultTable;
    //SQMTableModel *resultTableModel;

};
#endif // MAINWINDOW_H
