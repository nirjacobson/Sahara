#include "mainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("fusion");

    Sahara::ModelWidgetExample::MainWindow w;
    w.show();
    return a.exec();
}
