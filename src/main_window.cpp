#include "main_window.h"
#include <qboxlayout.h>
#include "play_widget.h"

namespace yk {

MainWindow::MainWindow(QWidget* parent) : ElaWindow(parent) {
	setIsNavigationBarEnable(false);
	InitView();
}

MainWindow::~MainWindow() {

}

void MainWindow::InitView() {
	// this->setCustomWidget 设置在标题栏上 位置上的
	
	/*vbox_play_layout_ = new QVBoxLayout();
	customWidget->setLayout(vbox_play_layout_);*/
	play_widget_ = new PlayWidget(this);
	//vbox_play_layout_->addWidget(play_widget_);

	addPageNode("HOME", play_widget_, ElaIconType::Backpack);
}

}