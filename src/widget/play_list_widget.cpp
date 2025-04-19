#include "play_list_widget.h"
#include <qboxlayout.h>
#include "play_list/media_list_widget.h"
#include "play_list/list_func_page.h"

namespace yk {

PlayListWidget::PlayListWidget(QWidget* parent) : QWidget(parent) {
	
	InitView();
}

PlayListWidget::~PlayListWidget() {

}

void PlayListWidget::InitView() {
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setStyleSheet("QWidget {background-color: #181818;}");
	setMinimumWidth(180);

	vbox_main_layout_ = new QVBoxLayout(this);
    vbox_main_layout_->setContentsMargins(0, 0, 0, 0);
    vbox_main_layout_->setSpacing(0);

	media_list_widget_ = new MediaListWidget(this); 
	vbox_main_layout_->addWidget(media_list_widget_);

	list_func_page_ = new ListFuncPage(this);
	vbox_main_layout_->addStretch(1);
	vbox_main_layout_->addWidget(list_func_page_);
}

}