#include "mainwindow.h"
#include "ui_mainwindow.h"

Sahara::ModelWidgetExample::MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->modelWidget, &Sahara::ModelWidget::initialized, this, &MainWindow::modelWidgetInitialized);
    connect(ui->actionScorpion, &QAction::triggered, this, &MainWindow::actionScorpionTriggered);
    connect(ui->actionTree, &QAction::triggered, this, &MainWindow::actionTreeTriggered);
}

Sahara::ModelWidgetExample::MainWindow::~MainWindow()
{
    delete ui;
}

void Sahara::ModelWidgetExample::MainWindow::modelWidgetInitialized()
{
    ui->modelWidget->setModel(":/models/scorpion.dae");
    ui->modelWidget->setAnimationClip("Idle");
}

void Sahara::ModelWidgetExample::MainWindow::actionScorpionTriggered()
{
    ui->modelWidget->setModel(":/models/scorpion.dae");
    ui->modelWidget->setAnimationClip("Idle");
    ui->actionScorpion->setChecked(true);
    ui->actionTree->setChecked(false);
}

void Sahara::ModelWidgetExample::MainWindow::actionTreeTriggered()
{
    ui->modelWidget->setModel(":/models/tree.dae");
    ui->actionScorpion->setChecked(false);
    ui->actionTree->setChecked(true);
}
