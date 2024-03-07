#include "mainwindow.h"
#include "ui_mainwindow.h"

Sahara::ModelWidgetExample::MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->modelWidget, &Sahara::ModelWidget::initialized, this, &MainWindow::modelWidgetInitialized);
    connect(ui->actionChibi, &QAction::triggered, this, &MainWindow::actionChibiTriggered);
    connect(ui->actionScorpion, &QAction::triggered, this, &MainWindow::actionScorpionTriggered);
    connect(ui->actionTree, &QAction::triggered, this, &MainWindow::actionTreeTriggered);
}

Sahara::ModelWidgetExample::MainWindow::~MainWindow()
{
    delete ui;
}

void Sahara::ModelWidgetExample::MainWindow::modelWidgetInitialized()
{
    // ui->actionChibi->trigger();
}

void Sahara::ModelWidgetExample::MainWindow::actionChibiTriggered()
{
    ui->modelWidget->setModel(":/models/chibi.dae");
    ui->modelWidget->setAnimationClip("Idle");

    ui->actionChibi->blockSignals(true);
    ui->actionChibi->setChecked(true);
    ui->actionChibi->blockSignals(false);

    ui->actionScorpion->blockSignals(true);
    ui->actionScorpion->setChecked(false);
    ui->actionScorpion->blockSignals(false);

    ui->actionScorpion->blockSignals(true);
    ui->actionTree->setChecked(false);
    ui->actionScorpion->blockSignals(false);
}

void Sahara::ModelWidgetExample::MainWindow::actionScorpionTriggered()
{
    ui->modelWidget->setModel(":/models/scorpion.dae");
    ui->modelWidget->setAnimationClip("Idle");

    ui->actionChibi->blockSignals(true);
    ui->actionChibi->setChecked(false);
    ui->actionChibi->blockSignals(false);

    ui->actionScorpion->blockSignals(true);
    ui->actionScorpion->setChecked(true);
    ui->actionScorpion->blockSignals(false);

    ui->actionScorpion->blockSignals(true);
    ui->actionTree->setChecked(false);
    ui->actionScorpion->blockSignals(false);
}

void Sahara::ModelWidgetExample::MainWindow::actionTreeTriggered()
{
    ui->modelWidget->setModel(":/models/tree.dae");

    ui->actionChibi->blockSignals(true);
    ui->actionChibi->setChecked(false);
    ui->actionChibi->blockSignals(false);

    ui->actionScorpion->blockSignals(true);
    ui->actionScorpion->setChecked(false);
    ui->actionScorpion->blockSignals(false);

    ui->actionScorpion->blockSignals(true);
    ui->actionTree->setChecked(true);
    ui->actionScorpion->blockSignals(false);
}
