#include "main_window.h"
#include <qboxlayout.h>
#include "public/base_page.h"
#include "play_widget.h"
#include "play_control_widget.h"
#include "play_list_widget.h"
#include "ElaTheme.h"

namespace yk {

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
	//setIsNavigationBarEnable(false);
	InitView();
}

MainWindow::~MainWindow() {

}

void MainWindow::InitView() {
	ElaTheme::getInstance()->setThemeMode(ElaThemeType::ThemeMode::Dark);
	//setWindowButtonFlag(ElaAppBarType::ButtonType::ThemeChangeButtonHint, false);
	//setWindowButtonFlag(ElaAppBarType::ButtonType::RouteBackButtonHint, false);
	
	//setStyleSheet("QWidget {background-color: #556600;}");
	// this->setCustomWidget 设置在标题栏上 位置上的

	resize(800, 600);

	bg_page_ = new QWidget(this);
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

	play_vbox_layout_->addWidget(play_widget_, 5);
	play_vbox_layout_->addWidget(play_control_widget_, 1);

	list_vbox_layout_ = new QVBoxLayout();
	list_vbox_layout_->setSpacing(0);
	list_vbox_layout_->setContentsMargins(0, 0, 0, 0);
	list_widget_ = new PlayListWidget(this);
	list_vbox_layout_->addWidget(list_widget_);

	main_hbox_layout_->addLayout(play_vbox_layout_, 5);
	main_hbox_layout_->addLayout(list_vbox_layout_, 1);
}

}