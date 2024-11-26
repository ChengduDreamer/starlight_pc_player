#include "play_widget.h"
#include <qstackedwidget.h>
#include <qsizepolicy.h>
#include <qboxlayout.h>
#include <qtimer.h>
#include "play_view.h"
#include "play_before_widget.h"
#include "context.h"
#include "app_messages.h"

namespace yk {

PlayWidget::PlayWidget(const std::shared_ptr<Context>& context, QWidget* parent) : context_(context), QWidget(parent) {
	msg_listener_ = context_->CreateMessageListener();
	InitView();
	InitSigChannels();
	RegisterEvents();
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

	play_view_ = new PlayView(context_);

	play_before_widget_ = new PlayBeforeWidget(context_);

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

void PlayWidget::InitSigChannels() {
	/*connect(play_before_widget_, &PlayBeforeWidget::SigOpenUrl, this, [this](QString url) {
		play_view_->Play(url);
	});*/




}

void PlayWidget::RegisterEvents() {
	if (!context_) {
		return;
	}

	msg_listener_->Listen<AppOpenUrlMsg>([=, this](const AppOpenUrlMsg& event) {
		context_->PostUITask([=, this]() {
			if (play_view_->Play(event.url)) {
				stacked_widget_->setCurrentWidget(play_view_);
			}
		});
	});
}


}