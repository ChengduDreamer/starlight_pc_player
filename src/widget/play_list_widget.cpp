#include "play_list_widget.h"
#include <qboxlayout.h>
#include "play_list/media_list_widget.h"
#include "play_list/list_func_page.h"
#include "context.h"
#include "app_messages.h"
namespace yk {

PlayListWidget::PlayListWidget(std::shared_ptr<Context> context, QWidget* parent) : QWidget(parent), context_(context) {
	
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

	media_list_widget_ = new MediaListWidget(context_, this);
	vbox_main_layout_->addWidget(media_list_widget_);

	list_func_page_ = new ListFuncPage(this);
	vbox_main_layout_->addStretch(1);
	vbox_main_layout_->addWidget(list_func_page_);
}

}