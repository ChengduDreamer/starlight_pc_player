#include "list_func_page.h"
#include <qsizepolicy.h>
#include <qboxlayout.h>
#include "yk_icon_button.h"


namespace yk {
ListFuncPage::ListFuncPage(QWidget* parent) : QWidget(parent) {
	InitView();
}

ListFuncPage::~ListFuncPage()
{

}

void ListFuncPage::InitView()
{
	auto size_policy = this->sizePolicy();
	size_policy.setHorizontalPolicy(QSizePolicy::Expanding);
    size_policy.setVerticalPolicy(QSizePolicy::Fixed);
	this->setFixedHeight(40);

	hbox_main_layout_ = new QHBoxLayout(this);
	hbox_main_layout_->setContentsMargins(0, 0, 0, 0);
	hbox_main_layout_->setSpacing(0);

	add_file_btn_ = new YKIconButton();
	add_file_btn_->Init(QSize(16, 16), ":/resource/list/add_file_normal.svg",
		":/resource/list/add_file_hover.svg", ":/resource/list/add_file_press.svg");
	add_file_btn_->SetUseSvg(true);

	clear_history_btn_ = new YKIconButton();
	clear_history_btn_->Init(QSize(16, 16), ":/resource/list/clear_normal.svg",
		":/resource/list/clear_hover.svg", ":/resource/list/clear_press.svg");
	clear_history_btn_->SetUseSvg(true);

	hbox_main_layout_->addStretch(1);

	hbox_main_layout_->addWidget(add_file_btn_);

	hbox_main_layout_->addSpacing(20);

	hbox_main_layout_->addWidget(clear_history_btn_);

	hbox_main_layout_->addSpacing(20);
}

}
