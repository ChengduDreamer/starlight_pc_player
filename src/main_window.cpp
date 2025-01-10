#include "main_window.h"
#include <qcoreapplication.h>
#include <qboxlayout.h>
#include <qguiapplication.h>
#include <qtimer.h>
#include <qfile.h>
#include <qdebug.h>
#include <qscreen.h>
#include <QWKWidgets/widgetwindowagent.h>
#include <widgetframe/windowbar.h>
#include <widgetframe/windowbutton.h>
#include "play_widget.h"
#include "play_control_widget.h"
#include "play_list_widget.h"
#include "bg_page.h"
#include "context.h"
#include "app_messages.h"
#include "cpp_base_lib/yk_logger.h"
#include "media_operation/media_operation.h"
//#include "media_operation/image_view.h"
#include "media_operation/image_panel.h"

namespace yk {

static inline void emulateLeaveEvent(QWidget* widget) {
    Q_ASSERT(widget);
    if (!widget) {
        return;
    }
    QTimer::singleShot(0, widget, [widget]() {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
        const QScreen* screen = widget->screen();
#else
        const QScreen* screen = widget->windowHandle()->screen();
#endif
        const QPoint globalPos = QCursor::pos(screen);
        if (!QRect(widget->mapToGlobal(QPoint{ 0, 0 }), widget->size()).contains(globalPos)) {
            QCoreApplication::postEvent(widget, new QEvent(QEvent::Leave));
            if (widget->testAttribute(Qt::WA_Hover)) {
                const QPoint localPos = widget->mapFromGlobal(globalPos);
                const QPoint scenePos = widget->window()->mapFromGlobal(globalPos);
                static constexpr const auto oldPos = QPoint{};
                const Qt::KeyboardModifiers modifiers = QGuiApplication::keyboardModifiers();
#if (QT_VERSION >= QT_VERSION_CHECK(6, 4, 0))
                const auto event =
                    new QHoverEvent(QEvent::HoverLeave, scenePos, globalPos, oldPos, modifiers);
                Q_UNUSED(localPos);
#elif (QT_VERSION >= QT_VERSION_CHECK(6, 3, 0))
                const auto event = new QHoverEvent(QEvent::HoverLeave, localPos, globalPos, oldPos, modifiers);
                Q_UNUSED(scenePos);
#else
                const auto event = new QHoverEvent(QEvent::HoverLeave, localPos, oldPos, modifiers);
                Q_UNUSED(scenePos);
#endif
                QCoreApplication::postEvent(widget, event);
            }
        }
    });
}


MainWindow::MainWindow(const std::shared_ptr<Context>& context, QWidget* parent) : context_(context), /*QMainWindow*/ QWidget(parent) {
    msg_listener_ = context_->CreateMessageListener();
	setAttribute(Qt::WA_DontCreateNativeAncestors);
	InstallWindowAgent(); // qt6.5 qt6.8 显示异常
    setMouseTracking(true);
	InitTitlebar();
	InitView();
    RegisterEvents();
    InitTimer();
   // centralWidget()->setMouseTracking(true);

    auto root_layout = new QVBoxLayout(this);
    root_layout->setSpacing(0);
    root_layout->setContentsMargins(0,0,0,0);
    root_layout->addWidget(window_bar_);
    root_layout->addWidget(bg_page_);

  
    located_screen_ = QGuiApplication::screenAt(this->pos());
    media_operation_ = MediaOperation::Make(context_);
}

MainWindow::~MainWindow() {

}

void MainWindow::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    QPen pen(QColor(0x00,0x00,0x00));
    pen.setWidth(8);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawLine(0, 0, width(), 0);
    //QMainWindow::paintEvent(event);
    QWidget::paintEvent(event);
}

void MainWindow::InitView() {
	
	resize(800, 600);

    if (QFile qss(QStringLiteral(":/dark-style.qss"));
        qss.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setStyleSheet(QString::fromUtf8(qss.readAll()));
    }
    
	bg_page_ = new BgPage(this);
    bg_page_->setMouseTracking(true);
    //setCentralWidget(bg_page_);
	bg_page_->setContentsMargins(0,0,0,0);
	//bg_page_->setStyleSheet("QWidget {background-color: #ff6632;}");
    bg_page_->setStyleSheet("QWidget {background-color: #0000ff;}");
    bg_page_->setMouseTracking(true);
    bg_page_->setMinimumHeight(200);
	main_hbox_layout_ = new QHBoxLayout();
	bg_page_->setLayout(main_hbox_layout_);
	main_hbox_layout_->setAlignment(Qt::AlignLeft);
	main_hbox_layout_->setSpacing(0);
	main_hbox_layout_->setContentsMargins(0,0,0,0);
	
	play_vbox_layout_ = new QVBoxLayout();
	play_vbox_layout_->setSpacing(0);
	play_vbox_layout_->setContentsMargins(0, 0, 0, 0);
	play_vbox_layout_->setAlignment(Qt::AlignTop);
	play_widget_ = new PlayWidget(context_, this);
	play_control_widget_ = new PlayControlWidget(context_, bg_page_);
    
	play_vbox_layout_->addWidget(play_widget_, 8);
	play_vbox_layout_->addWidget(play_control_widget_, 1);
#if 0
	list_vbox_layout_ = new QVBoxLayout();
	list_vbox_layout_->setSpacing(0);
	list_vbox_layout_->setContentsMargins(0, 0, 0, 0);
	list_widget_ = new PlayListWidget(this);
	list_vbox_layout_->addWidget(list_widget_);
#endif
	main_hbox_layout_->addLayout(play_vbox_layout_, 6);
	///main_hbox_layout_->addLayout(list_vbox_layout_, 1);

    /*image_view_ = ImageView::Make(context_);
    image_view_->hide();*/

    image_panel_ = new ImagePanel(context_);
    image_panel_->hide();
    //image_panel_->show();
}

void MainWindow::InstallWindowAgent() {
	windowAgent = new QWK::WidgetWindowAgent(this);
	windowAgent->setup(this);
}

void MainWindow::InitTitlebar() {
    auto menuBar = [this]() {
        auto menuBar = new QMenuBar(this);
        auto file = new QMenu(tr("File(&F)"), menuBar);
        file->addAction(new QAction(tr("New(&N)"), menuBar));
        file->addAction(new QAction(tr("Open(&O)"), menuBar));
        file->addSeparator();
        menuBar->addMenu(file);
        return menuBar;
        }();
    menuBar->setObjectName(QStringLiteral("win-menu-bar"));

    auto titleLabel = new QLabel();
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName(QStringLiteral("win-title-label"));
    titleLabel->setText(QStringLiteral("愿你的世界充满星光"));
    //titleLabel->setStyleSheet("QLabel {}");

    auto iconButton = new QWK::WindowButton();
    iconButton->setObjectName(QStringLiteral("icon-button"));
    iconButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto min_btn = new QPushButton(this);
    min_btn->setObjectName("minimizeButton");
    min_btn->setToolTip(QObject::tr("最小化"));
    min_btn->setFixedSize(30, 20);
    // 样式表设置padding: 4px; 是为了不然svg填充整个图片
    min_btn->setStyleSheet("QPushButton {image: url(:/resource/window/minimize.svg); padding: 4px; border: 0px; background-color:#000000;} QPushButton:hover{background-color:#333333;} QPushButton:pressed{background-color:#555555;}");

    auto max_btn = new QPushButton(this);
    max_btn->setToolTip(QObject::tr("最大化"));
    max_btn->setFixedSize(30, 20);
    max_btn->setStyleSheet("QPushButton:checked{image: url(:/resource/window/restore.svg);} QPushButton {padding: 4px; border: 0px; background-color:#000000; image: url(:/resource/window/maximize.svg);} QPushButton:hover{background-color:#333333;} QPushButton:pressed{background-color:#555555;}");
    max_btn->setCheckable(true);

    auto close_btn = new QPushButton(this);
    close_btn->setFixedSize(30, 20);
    close_btn->setStyleSheet("QPushButton {image:url(:/resource/window/close.svg); border: 0px; background-color:#000000; padding: 4px;} QPushButton:hover{background-color:#333333;} QPushButton:pressed{background-color:#555555;}");
    close_btn->setObjectName("closeButton");
    close_btn->setToolTip(QObject::tr("关闭"));

    window_bar_ = new QWK::WindowBar();
    window_bar_->setStyleSheet("QWidget {background-color:#000000;}");
    window_bar_->setAttribute(Qt::WA_StyledBackground);
    window_bar_->setIconButton(iconButton);
    window_bar_->setMinButton(min_btn);
    window_bar_->setMaxButton(max_btn);
    window_bar_->setCloseButton(close_btn);
    window_bar_->setMenuBar(menuBar);
    window_bar_->setTitleLabel(titleLabel);
    window_bar_->setHostWidget(this);
    windowAgent->setTitleBar(window_bar_);
    windowAgent->setSystemButton(QWK::WindowAgentBase::WindowIcon, iconButton);
    windowAgent->setSystemButton(QWK::WindowAgentBase::Minimize, min_btn);
    windowAgent->setSystemButton(QWK::WindowAgentBase::Maximize, max_btn);
    windowAgent->setSystemButton(QWK::WindowAgentBase::Close, close_btn);
    windowAgent->setHitTestVisible(menuBar, true);

    //setMenuWidget(window_bar_);
    connect(window_bar_, &QWK::WindowBar::minimizeRequested, this, &QWidget::showMinimized);
    connect(window_bar_, &QWK::WindowBar::maximizeRequested, this, [this, max_btn](bool max) {
        if (max) {
            showMaximized();
        }
        else {
            showNormal();
        }

        // It's a Qt issue that if a QAbstractButton::clicked triggers a window's maximization,
        // the button remains to be hovered until the mouse move. As a result, we need to
        // manually send leave events to the button.
        emulateLeaveEvent(max_btn);
        });
    connect(window_bar_, &QWK::WindowBar::closeRequested, this, &QWidget::close);
}

void MainWindow::RegisterEvents() {
    msg_listener_->Listen<AppFullScreenMsg>([=, this](const AppFullScreenMsg& event) {
        context_->PostUITask([=, this]() {
            timer_->start();
            window_bar_->hide();
            //list_widget_->hide();
            YK_LOGI("showFullScreen");
            this->showFullScreen();
            AppFullScreenTakeEffectMsg te_msg{};
            context_->SendAppMessage(te_msg);
        });
    });

    msg_listener_->Listen<AppExitFullScreenMsg>([=, this](const AppExitFullScreenMsg& event) {
        context_->PostUITask([=, this]() {
            timer_->stop();
            window_bar_->show();
            //list_widget_->show();
            YK_LOGI("exitFullScreen");
            this->showNormal();
            play_control_widget_->show();
            AppExitFullScreenTakeEffectMsg te_msg{};
            context_->SendAppMessage(te_msg);
        });
    });

    msg_listener_->Listen<AppPlayViewMouseDoubleClickedMsg>([=, this](const AppPlayViewMouseDoubleClickedMsg& event) {
        context_->PostUITask([=, this]() {
            if (this->isFullScreen()) {
                AppExitFullScreenMsg msg{};
                context_->SendAppMessage(msg);
            }
            else {
                AppFullScreenMsg msg{};
                context_->SendAppMessage(msg);
            }
        });
     });

    msg_listener_->Listen<AppPlayViewKeyEscMsg>([=, this](const AppPlayViewKeyEscMsg& event) {
        context_->PostUITask([=, this]() {
            qDebug() << "Listen AppPlayViewKeyEscMsg";
            if (this->isFullScreen()) {
                AppExitFullScreenMsg msg{};
                context_->SendAppMessage(msg);
            }
        });
    });

    msg_listener_->Listen<AppMainWindowResizeMsg>([=, this](const AppMainWindowResizeMsg& event) {
        context_->PostUITask([=, this]() {
            QTimer::singleShot(1000, [this]() {
                auto w = this->width();
                auto h = this->height();
                resize(++w, ++h);
                update();
                resize(--w, --h);
                update();
            });
        });
    });

    // test

    //auto image_view = new ImageView(context_);
    //image_view->hide();
    msg_listener_->Listen<AppCaptureImageCompletedMsg>([=, this](const AppCaptureImageCompletedMsg& event) {
        context_->PostUITask([=, this]() {
            qDebug() << "mainwindow AppCaptureImageCompletedMsg";
            if (!event.success) {
                qDebug() << "mainwindow AppCaptureImageCompletedMsg error";
                return;
            }
            /*image_view->SetPixmap(event.pixmap);
            image_view->show();*/
            //image_panel_->show();

            image_panel_->showMaximized();
        });
    });

    this->installEventFilter(this);
}

void MainWindow::InitTimer() {
    timer_ = new QTimer();
    connect(timer_, &QTimer::timeout, this, [this]() {
        if (this->isFullScreen()) {
            play_control_widget_->hide();
        }
    });
    timer_->setInterval(3000);
}

void MainWindow::moveEvent(QMoveEvent* event) {
    QPoint new_pos = event->pos();
    QScreen* current_screen = QGuiApplication::screenAt(new_pos);
    if (current_screen) {
        if (located_screen_ != current_screen) {
            located_screen_ = current_screen;
            QTimer::singleShot(1000, [this]() {
                auto w = this->width();
                auto h = this->height();
                resize(++w, ++h);
                update();
                resize(--w, --h);
                update();
            });
        }
    }
    return QWidget::moveEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    if (this->isFullScreen()) {
        play_control_widget_->show();
    }
    //return QMainWindow::mouseMoveEvent(event);
    return QWidget::mouseMoveEvent(event);
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    return QWidget::eventFilter(obj, event);
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        qDebug() << "MainWindow keyPressEvent Key_Escape";
        AppPlayViewKeyEscMsg msg;
        context_->SendAppMessage(msg);
    }
    else if (event->key() == Qt::Key_Space) {
        qDebug() << "MainWindow keyPressEvent Key_Space";
        AppPlayViewKeySpaceMsg msg;
        context_->SendAppMessage(msg);
    }
    QWidget::keyPressEvent(event);
}

}