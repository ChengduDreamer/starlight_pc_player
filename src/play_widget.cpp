#include "play_widget.h"
#include <qstackedwidget.h>
#include <qsizepolicy.h>
#include <qboxlayout.h>
#include <qtimer.h>
#include "play_view.h"
#include "play_before_widget.h"

namespace yk {

PlayWidget::PlayWidget(QWidget* parent) : QWidget(parent) {
	InitView();
}


PlayWidget::~PlayWidget() {

}

void PlayWidget::InitView() {
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setStyleSheet(".QWidget {background-color: #000000;}");
	QVBoxLayout* main_vbox_layout = new QVBoxLayout();
	main_vbox_layout->setSpacing(0);
	main_vbox_layout->setContentsMargins(0,0,0,0);
	main_vbox_layout->setAlignment(Qt::AlignTop);
	this->setLayout(main_vbox_layout);


	stacked_widget_ = new QStackedWidget();
	stacked_widget_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	play_view_ = new PlayView();

	play_before_widget_ = new PlayBeforeWidget();

	stacked_widget_->addWidget(play_before_widget_);
	stacked_widget_->addWidget(play_view_);
	stacked_widget_->setCurrentWidget(play_before_widget_);

	// test, to do del
	play_view_->setStyleSheet(".QWidget {background-color: #888888;}");

	main_vbox_layout->addWidget(stacked_widget_);

	//QTimer::singleShot(1000, [=]() {
	//	play_view_->Play();	
	//});
	
}

}