#include <iostream>
#include <memory>
#include <QApplication>
#include <QScreen>
#include <qfile.h>
#include "main_window.h"
#include "context.h"
#include "yk_logger.h"

#include "qdrag_widget.h"
#include "draw_board/draw_board_widget.h"
#include "data_base/data_base.h"

using namespace yk;

int main(int argc, char* argv[])
{
#ifdef WIN32
    // 设置命令行窗口的输出编码为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    // 设置输入编码为 UTF-8（可选）
    SetConsoleCP(CP_UTF8);
#endif // WIN32

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QCoreApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QApplication a(argc, argv);

    std::cout << "main thread id = " << std::this_thread::get_id() << std::endl;

    std::shared_ptr<Context> context = std::make_shared<Context>();
    if (!context->Init()) {
        YK_LOGE("Context init error.");
        return -1;
    }
    Database::Instance();

    std::shared_ptr<MainWindow> main_window = std::make_shared<MainWindow>(context);
    main_window->show();

    

    return a.exec();
}
