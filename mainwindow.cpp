#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Content Searcher");
//    QAction *actionExit = ui->menubar->addMenu("File")->addAction("Exit");

    connect(ui->menubar->addMenu("File")->addAction("Exit"), &QAction::triggered, this, &QMainWindow::close);
    connect(ui->menubar->addMenu("Fil")->addAction("qt"), &QAction::triggered, this, &MainWindow::renameSelf);
}

void MainWindow::renameSelf()
{
    setWindowTitle("Content Searcher Qt");
}

MainWindow::~MainWindow()
{
    delete ui;
}

