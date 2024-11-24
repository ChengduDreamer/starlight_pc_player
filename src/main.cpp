#include <iostream>
#include <QApplication>
#include <QScreen>
#include <qfile.h>
#include "main_window.h"
// test
#include "play_view.h"

using namespace yk;

int main(int argc, char* argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QCoreApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QApplication a(argc, argv);

 

    MainWindow w;
    w.show();

    return a.exec();
}
