#include <iostream>
#include <QApplication>
#include <QScreen>

#include "ElaApplication.h"
#include "main_window.h"

using namespace sp;

int main(int argc, char* argv[])
{
    // ������������Ҫ ������Ч��
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication a(argc, argv);
    eApp->init();
    MainWindow w;
    w.show();
    return a.exec();
}
