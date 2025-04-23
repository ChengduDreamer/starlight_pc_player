#include "play_widget.h"
#include <qstackedwidget.h>
#include <qsizepolicy.h>
#include <qboxlayout.h>
#include <qtimer.h>
#include "play_view.h"
#include "play_before_widget.h"
#include "context.h"
#include "app_messages.h"
#include "cpp_base_lib/yk_logger.h"

namespace yk {

PlayWidget::PlayWidget(const std::shared_ptr<Context>& context, QWidget* parent) : context_(context), QWidget(parent) {
	msg_listener_ = context_->CreateMessageListener();
	setMouseTracking(true);
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
	stacked_widget_->setMouseTracking(true); // QStackedWidget 也需要设置setMouseTracking 不然play_view_ 无法接收到鼠标移动事件

	play_view_ = new PlayView(context_, this);
	play_view_->setMouseTracking(true);
	play_before_widget_ = new PlayBeforeWidget(context_);
	play_before_widget_->setMouseTracking(true);
	stacked_widget_->addWidget(play_before_widget_);
	stacked_widget_->addWidget(play_view_);
	stacked_widget_->setCurrentWidget(play_before_widget_);



	main_vbox_layout->addWidget(stacked_widget_);
}

void PlayWidget::InitSigChannels() {
	


}

void PlayWidget::RegisterEvents() {
	if (!context_) {
		return;
	}

	msg_listener_->Listen<AppLibvlcMediaPlayerPlayingMsg>([=, this](const AppLibvlcMediaPlayerPlayingMsg& event) {
		context_->PostUITask([=, this]() {
			stacked_widget_->setCurrentWidget(play_view_);
		});
	});

	msg_listener_->Listen<AppOpenUrlMsg>([=, this](const AppOpenUrlMsg& event) {
		context_->PostUITask([=, this]() {
			play_view_->Play(event.url);
		});
	});

	msg_listener_->Listen<AppLibvlcMediaPlayerEncounteredErrorMsg>([=, this](const AppLibvlcMediaPlayerEncounteredErrorMsg& event) {
		context_->PostUITask([=, this]() {
			stacked_widget_->setCurrentWidget(play_before_widget_);
			// to do 弹窗 
		});
	});

	msg_listener_->Listen<AppStopPlayMsg>([=, this](const AppStopPlayMsg& event) {
		context_->PostUITask([=, this]() {
			stacked_widget_->setCurrentWidget(play_before_widget_);
			play_view_->Stop();
		});
	});

	msg_listener_->Listen<AppPausePlayMsg>([=, this](const AppPausePlayMsg& event) {
		context_->PostUITask([=, this]() {
			play_view_->Pause();
		});
	});

	msg_listener_->Listen<AppResumePlayMsg>([=, this](const AppResumePlayMsg& event) {
		context_->PostUITask([=, this]() {
			play_view_->Resume();
		});
	});

	msg_listener_->Listen<AppSeekPosMsg>([=, this](const AppSeekPosMsg& event) {
		context_->PostUITask([=, this]() {
			play_view_->SeekPos(event.pos);
		});
	});

	msg_listener_->Listen<AppSetMuteMsg>([=, this](const AppSetMuteMsg& event) {
		context_->PostUITask([=, this]() {
			play_view_->SetMute();
		});
	});

	msg_listener_->Listen<AppSetUnmuteMsg>([=, this](const AppSetUnmuteMsg& event) {
		context_->PostUITask([=, this]() {
			play_view_->SetUnmute();
		});
	});

	msg_listener_->Listen<AppSetVolumeMsg>([=, this](const AppSetVolumeMsg& event) {
		context_->PostUITask([=, this]() {
			play_view_->SetVolume(event.volume);
		});
	});
}


}