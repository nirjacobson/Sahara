#include "mainwindow.h"
#include "ui_mainwindow.h"

Sahara::ModelWidgetExample::MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSettings settings("Nir Jacobson", "ModelWidgetExample");
    _vulkan = settings.value("API").toString() == "Vulkan" && QVulkanInstance().create();
    ui->actionOpenGL->setChecked(!_vulkan);
    ui->actionVulkan->setChecked(_vulkan);

    _modelWidget = new Sahara::ModelWidget(this, _vulkan);

    ui->centralwidget->layout()->replaceWidget(ui->modelWidget, _modelWidget);

    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(_modelWidget, &Sahara::ModelWidget::initialized, this, &MainWindow::modelWidgetInitialized);
    connect(ui->actionChibi, &QAction::triggered, this, &MainWindow::actionChibiTriggered);
    connect(ui->actionScorpion, &QAction::triggered, this, &MainWindow::actionScorpionTriggered);
    connect(ui->actionTree, &QAction::triggered, this, &MainWindow::actionTreeTriggered);
    connect(ui->actionOpenGL, &QAction::triggered, this, &MainWindow::actionOpenGLTriggered);
    connect(ui->actionVulkan, &QAction::triggered, this, &MainWindow::actionVulkanTriggered);
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

void Sahara::ModelWidgetExample::MainWindow::actionOpenGLTriggered()
{
    ui->actionOpenGL->setChecked(!_vulkan);
    ui->actionVulkan->setChecked(_vulkan);

    QSettings settings("Nir Jacobson", "ModelWidgetExample");
    settings.setValue("API", "OpenGL");

    QMessageBox::information(this, "Please restart the program", "Please restart the program for the changes to take effect.");
}

void Sahara::ModelWidgetExample::MainWindow::actionVulkanTriggered()
{
    ui->actionOpenGL->setChecked(!_vulkan);
    ui->actionVulkan->setChecked(_vulkan);

    QSettings settings("Nir Jacobson", "ModelWidgetExample");
    settings.setValue("API", "Vulkan");

    QMessageBox::information(this, "Please restart the program", "Please restart the program for the changes to take effect.");
}
