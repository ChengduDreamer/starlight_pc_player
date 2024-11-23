#include <iostream>
#include <QApplication>
#include <QScreen>
#include "main_window.h"

// test
#include "play_view.h"

using namespace yk;

int main(int argc, char* argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
