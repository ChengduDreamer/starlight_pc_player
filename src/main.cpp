#include <iostream>
#include <memory>
#include <QApplication>
#include <QScreen>
#include <qfile.h>
#include "main_window.h"
#include "context.h"
#include "yk_logger.h"

#include "qdrag_widget.h"
#include "draw_board/CDrawBoardWidget.h"

using namespace yk;

int main(int argc, char* argv[])
{
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

    //std::shared_ptr<MainWindow> main_window = std::make_shared<MainWindow>(context);
    //main_window->show();

   // QDragWidget drag_widget{ context };
   //
   // drag_widget.show();

    CDrawBoardWidget w;
    w.show();

    return a.exec();
}
