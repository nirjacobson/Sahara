#include "mainwindow.h"
#include "ui_mainwindow.h"

Sahara::ModelWidgetExample::MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _modelWidget = new Sahara::ModelWidget(this, true);

    ui->centralwidget->layout()->replaceWidget(ui->modelWidget, _modelWidget);

    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(_modelWidget, &Sahara::ModelWidget::initialized, this, &MainWindow::modelWidgetInitialized);
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
    ui->actionChibi->trigger();
}

void Sahara::ModelWidgetExample::MainWindow::actionChibiTriggered()
{
    _modelWidget->setModel(":/models/chibi.dae");
    _modelWidget->setAnimationClip("Idle");

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
    _modelWidget->setModel(":/models/scorpion.dae");
    _modelWidget->setAnimationClip("Idle");

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
    _modelWidget->setModel(":/models/tree.dae");

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
