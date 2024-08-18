#include "mainwindow.h"
#include "ui_mainwindow.h"

const QString Sahara::ModelWidgetExample::MainWindow::Organization = "Nir Jacobson";
const QString Sahara::ModelWidgetExample::MainWindow::Application = "ModelWidgetExample";
const QString Sahara::ModelWidgetExample::MainWindow::SettingsFile = "settings.ini";

Sahara::ModelWidgetExample::MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#ifdef Q_OS_WIN
    QSettings settings(SettingsFile, QSettings::IniFormat);
#else
    QSettings settings(Organization, Application);
#endif
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
    if (ui->actionChibi->isChecked()) {
        ui->actionChibi->trigger();
    } else if (ui->actionScorpion->isChecked()) {
        ui->actionScorpion->trigger();
    } else if (ui->actionTree->isChecked()) {
        ui->actionTree->trigger();
    } else {
        ui->actionChibi->trigger();
    }
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

#ifdef Q_OS_WIN
    QSettings settings(SettingsFile, QSettings::IniFormat);
#else
    QSettings settings(Organization, Application);
#endif

    settings.setValue("API", "OpenGL");

    QMessageBox::information(this, "Please restart the program", "Please restart the program for the changes to take effect.");
}

void Sahara::ModelWidgetExample::MainWindow::actionVulkanTriggered()
{
    ui->actionOpenGL->setChecked(!_vulkan);
    ui->actionVulkan->setChecked(_vulkan);

#ifdef Q_OS_WIN
    QSettings settings(SettingsFile, QSettings::IniFormat);
#else
    QSettings settings(Organization, Application);
#endif

    settings.setValue("API", "Vulkan");

    QMessageBox::information(this, "Please restart the program", "Please restart the program for the changes to take effect.");
}
