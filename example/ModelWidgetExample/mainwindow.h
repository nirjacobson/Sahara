#ifndef SAHARA_MODELWIDGETEXAMPLE_MAINWINDOW_H
#define SAHARA_MODELWIDGETEXAMPLE_MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMessageBox>

#include "widgets/modelwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace Sahara {

    namespace ModelWidgetExample {

        class MainWindow : public QMainWindow
        {
                Q_OBJECT

            public:
                MainWindow(QWidget *parent = nullptr);
                ~MainWindow();

            private:
                static const QString Organization;
                static const QString Application;
                static const QString SettingsFile;

                Ui::MainWindow *ui;
                bool _vulkan;
                Sahara::ModelWidget* _modelWidget;

            private slots:
                void modelWidgetInitialized();
                void actionChibiTriggered();
                void actionScorpionTriggered();
                void actionTreeTriggered();
                void actionOpenGLTriggered();
                void actionVulkanTriggered();
        };

    }

}
#endif // SAHARA_MODELWIDGETEXAMPLE_MAINWINDOW_H
