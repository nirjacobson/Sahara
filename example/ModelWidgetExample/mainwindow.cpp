#include "mainwindow.h"
#include "ui_mainwindow.h"

Sahara::ModelWidgetExample::MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->modelWidget, &Sahara::ModelWidget::initialized, this, &MainWindow::modelWidgetInitialized);
}

Sahara::ModelWidgetExample::MainWindow::~MainWindow()
{
    delete ui;
}

void Sahara::ModelWidgetExample::MainWindow::modelWidgetInitialized()
{
    ui->modelWidget->setModel(":/models/scorpion.dae");
}
