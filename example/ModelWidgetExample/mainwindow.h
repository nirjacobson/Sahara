#ifndef SAHARA_MODELWIDGETEXAMPLE_MAINWINDOW_H
#define SAHARA_MODELWIDGETEXAMPLE_MAINWINDOW_H

#include <QMainWindow>

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
                Ui::MainWindow *ui;

            private slots:
                void modelWidgetInitialized();
                void actionChibiTriggered();
                void actionScorpionTriggered();
                void actionTreeTriggered();
        };

    }

}
#endif // SAHARA_MODELWIDGETEXAMPLE_MAINWINDOW_H
