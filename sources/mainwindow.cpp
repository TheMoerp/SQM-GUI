#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    spinBase = ui->spinBase;
    spinExp = ui->spinExp;
    spinMod = ui->spinMod;


    resultTable = ui->resultTable;
    resultTableModel = new SQMTableModel;
//    resultTable->setModel(resultTableModel);
//    resultTable->show();

    connect(spinBase, SIGNAL(valueChanged(int)), this, SLOT(SetModel()));
    connect(spinExp, SIGNAL(valueChanged(int)), this, SLOT(SetModel()));
    connect(spinMod, SIGNAL(valueChanged(int)), this, SLOT(SetModel()));



}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::SetModel() {
    int base = spinBase->value();
    int exp = spinExp->value();
    int mod = spinMod->value();


    resultTableModel->SetStartValues(base, exp, mod);
    resultTable->setModel(resultTableModel);
    //resultTable->show();
}
