#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("fusion");
    Sahara::ModelWidgetExample::MainWindow w;
    w.showFullScreen();
    return a.exec();
}
