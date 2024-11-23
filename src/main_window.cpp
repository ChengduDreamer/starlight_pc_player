#include "main_window.h"
#include <qboxlayout.h>
#include "play_widget.h"
#include "play_control_widget.h"
#include "play_list_widget.h"

namespace yk {

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
	InitView();
}

MainWindow::~MainWindow() {

}

void MainWindow::InitView() {
	
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

	play_vbox_layout_->addWidget(play_widget_, 8);
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