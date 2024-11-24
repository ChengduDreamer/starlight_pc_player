#include "main_window.h"
#include <qcoreapplication.h>
#include <qboxlayout.h>
#include <qguiapplication.h>
#include <qtimer.h>
#include <qfile.h>
#include <QWKWidgets/widgetwindowagent.h>
#include <widgetframe/windowbar.h>
#include <widgetframe/windowbutton.h>
#include "play_widget.h"
#include "play_control_widget.h"
#include "play_list_widget.h"

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


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
	setAttribute(Qt::WA_DontCreateNativeAncestors);
	//InstallWindowAgent(); // qt6.5 qt6.8 显示异常
	//InitTitlebar();
	InitView();
}

MainWindow::~MainWindow() {

}

void MainWindow::InitView() {
	
	resize(800, 600);

    /*if (QFile qss(QStringLiteral(":/dark-style.qss"));
        qss.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setStyleSheet(QString::fromUtf8(qss.readAll()));
    }*/


	bg_page_ = new QWidget();
    setCentralWidget(bg_page_);
	bg_page_->setContentsMargins(0,0,0,0);
	bg_page_->setStyleSheet("QWidget {background-color: #ff6632;}");
	main_hbox_layout_ = new QHBoxLayout();
	bg_page_->setLayout(main_hbox_layout_);
	main_hbox_layout_->setAlignment(Qt::AlignLeft);
	main_hbox_layout_->setSpacing(0);
	main_hbox_layout_->setContentsMargins(0,0,0,0);
	
	play_vbox_layout_ = new QVBoxLayout();
	play_vbox_layout_->setSpacing(0);
	play_vbox_layout_->setContentsMargins(0, 0, 0, 0);
	play_vbox_layout_->setAlignment(Qt::AlignTop);
	play_widget_ = new PlayWidget(this);
    
	play_control_widget_ = new PlayControlWidget(this);
    
	play_vbox_layout_->addWidget(play_widget_, 8);
	play_vbox_layout_->addWidget(play_control_widget_, 1);
    
	list_vbox_layout_ = new QVBoxLayout();
	list_vbox_layout_->setSpacing(0);
	list_vbox_layout_->setContentsMargins(0, 0, 0, 0);
	list_widget_ = new PlayListWidget(this);
	list_vbox_layout_->addWidget(list_widget_);
    
	main_hbox_layout_->addLayout(play_vbox_layout_, 6);
	main_hbox_layout_->addLayout(list_vbox_layout_, 1);
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
    titleLabel->setText(QStringLiteral("星光播放器"));

    auto iconButton = new QWK::WindowButton();
    iconButton->setObjectName(QStringLiteral("icon-button"));
    iconButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto minButton = new QWK::WindowButton();
    minButton->setObjectName(QStringLiteral("min-button"));
    minButton->setProperty("system-button", true);
    minButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto maxButton = new QWK::WindowButton();
    maxButton->setCheckable(true);
    maxButton->setObjectName(QStringLiteral("max-button"));
    maxButton->setProperty("system-button", true);
    maxButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto closeButton = new QWK::WindowButton();
    closeButton->setObjectName(QStringLiteral("close-button"));
    closeButton->setProperty("system-button", true);
    closeButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto windowBar = new QWK::WindowBar();
    windowBar->setStyleSheet("QWidget {background-color:#000000;}");
    windowBar->setAttribute(Qt::WA_StyledBackground);
    windowBar->setIconButton(iconButton);
    windowBar->setMinButton(minButton);
    windowBar->setMaxButton(maxButton);
    windowBar->setCloseButton(closeButton);
    windowBar->setMenuBar(menuBar);
    windowBar->setTitleLabel(titleLabel);
    windowBar->setHostWidget(this);
    windowAgent->setTitleBar(windowBar);
    windowAgent->setSystemButton(QWK::WindowAgentBase::WindowIcon, iconButton);
    windowAgent->setSystemButton(QWK::WindowAgentBase::Minimize, minButton);
    windowAgent->setSystemButton(QWK::WindowAgentBase::Maximize, maxButton);
    windowAgent->setSystemButton(QWK::WindowAgentBase::Close, closeButton);
    windowAgent->setHitTestVisible(menuBar, true);

    setMenuWidget(windowBar);
    connect(windowBar, &QWK::WindowBar::minimizeRequested, this, &QWidget::showMinimized);
    connect(windowBar, &QWK::WindowBar::maximizeRequested, this, [this, maxButton](bool max) {
        if (max) {
            showMaximized();
        }
        else {
            showNormal();
        }

        // It's a Qt issue that if a QAbstractButton::clicked triggers a window's maximization,
        // the button remains to be hovered until the mouse move. As a result, we need to
        // manually send leave events to the button.
        emulateLeaveEvent(maxButton);
        });
    connect(windowBar, &QWK::WindowBar::closeRequested, this, &QWidget::close);
}

}