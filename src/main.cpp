#include <iostream>
#include <QApplication>
#include <QScreen>

#include "ElaApplication.h"
#include "main_window.h"

// test
#include "play_view.h"

using namespace yk;

int main(int argc, char* argv[])
{
    // 下面这两行需要 测试下效果
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication a(argc, argv);
    eApp->init();
    MainWindow w;
    w.show();

    // 这样能播放
    //auto p = new PlayView();
    //p->show();
    //p->Play();

    return a.exec();
}
