#include "play_widget.h"
#include <qstackedwidget.h>
#include <qsizepolicy.h>
#include "play_view.h"

namespace yk {

PlayWidget::PlayWidget(QWidget* parent) : QWidget(parent) {
	InitView();
}


PlayWidget::~PlayWidget() {

}

void PlayWidget::InitView() {
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setStyleSheet(".QWidget {background-color: #000000;}");

	stacked_widget_ = new QStackedWidget(this);
	stacked_widget_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	play_view_ = new PlayView(this);
	play_view_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	stacked_widget_->addWidget(play_view_);
}

}