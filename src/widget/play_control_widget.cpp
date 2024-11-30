#include "play_control_widget.h"
#include <qsizepolicy.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <qboxlayout.h>
#include <qpixmap.h>
#include <qtimer.h>

#include "public/yk_icon_button.h"
#include "public/yk_progress_bar.h"
#include "public/yk_style_sheet.h"
#include "app_messages.h"
#include "context.h"

namespace yk {

PlayControlWidget::PlayControlWidget(const std::shared_ptr<Context>& context, QWidget* parent) : context_(context), QWidget(parent) {
	InitView();
	InitSignalChannels();
	RegisterEvents();
}

PlayControlWidget::~PlayControlWidget() {

}

void PlayControlWidget::InitView() {
	msg_listener_ = context_->CreateMessageListener();
	setFixedHeight(60);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	setAttribute(Qt::WA_StyledBackground);
	setStyleSheet("QWidget {background-color: #000000;}");

	QVBoxLayout* main_vbox_layout = new QVBoxLayout(this);
	main_vbox_layout->setAlignment(Qt::AlignTop);
	main_vbox_layout->setContentsMargins(0,0,0,0);
	main_vbox_layout->setSpacing(0);
	
	play_progress_bar_ = new YKProgressBar();
	play_progress_bar_->setFixedHeight(12);
	play_progress_bar_->setOrientation(Qt::Horizontal); //不设置这一项，样式表无效
	play_progress_bar_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	play_progress_bar_->setAttribute(Qt::WA_StyledBackground);
	play_progress_bar_->setStyleSheet(QString::fromStdString(KPlayerSliderCss));

	main_vbox_layout->addSpacing(4);
	main_vbox_layout->addWidget(play_progress_bar_);

	QHBoxLayout* control_hbox_layout = new QHBoxLayout();
	control_hbox_layout->setContentsMargins(0,0,0,0);
	control_hbox_layout->setAlignment(Qt::AlignLeft);
	control_hbox_layout->setSpacing(0);

	pos_dur_widget_ = new QWidget();
	auto pos_dur_policy = pos_dur_widget_->sizePolicy();
	pos_dur_policy.setRetainSizeWhenHidden(true);
	pos_dur_widget_->setSizePolicy(pos_dur_policy);
	pos_dur_widget_->hide();
	auto pos_dur_hbox_layout = new QHBoxLayout(pos_dur_widget_);
	pos_dur_hbox_layout->setSpacing(4);
	pos_dur_hbox_layout->setAlignment(Qt::AlignLeft);
	pos_lab_ = new QLabel();
	pos_lab_->setText("00:00:00");
	pos_lab_->setStyleSheet("QLabel {background-color: transparent; font-family:Microsoft YaHei;font-size:12px; color:#ffffff;}");
	auto split_lab = new QLabel();
	split_lab->setText("/");
	split_lab->setStyleSheet("QLabel {background-color: transparent; font-family:Microsoft YaHei;font-size:12px; color:#ffffff;}");
	duration_lab_ = new QLabel();
	duration_lab_->setText("00:00:00");
	duration_lab_->setStyleSheet("QLabel {background-color: transparent; font-family:Microsoft YaHei;font-size:12px; color:#ffffff;}");
	pos_dur_hbox_layout->addWidget(pos_lab_);
	pos_dur_hbox_layout->addWidget(split_lab);
	pos_dur_hbox_layout->addWidget(duration_lab_);
	
	stop_btn_ = new YKIconButton();
	stop_btn_->Init(QSize(26, 26), ":/resource/control/stop_normal.svg",
		":/resource/control/stop_hover.svg", ":/resource/control/stop_press.svg");
	stop_btn_->SetUseSvg(true);

	previous_btn_ = new YKIconButton();
	previous_btn_->Init(QSize(26, 26), ":/resource/control/skip_previous_normal.svg",
		":/resource/control/skip_previous_hover.svg", ":/resource/control/skip_previous_press.svg");

	start_pause_btn_stack_ = new QStackedWidget();
	start_btn_ = new YKIconButton();
	start_btn_->Init(QSize(30, 30), ":/resource/control/play_normal.svg",
		":/resource/control/play_hover.svg", ":/resource/control/play_press.svg");

	pause_btn_ = new YKIconButton();
	pause_btn_->Init(QSize(30, 30), ":/resource/control/pause_normal.svg",
		":/resource/control/pause_hover.svg", ":/resource/control/pause_press.svg");

	start_pause_btn_stack_->addWidget(pause_btn_);
	start_pause_btn_stack_->addWidget(start_btn_);
	start_pause_btn_stack_->setCurrentWidget(start_btn_);
	start_pause_btn_stack_->setFixedSize(pause_btn_->size());

	next_btn_ = new YKIconButton();
	next_btn_->Init(QSize(26, 26), ":/resource/control/skip_next_normal.svg",
		":/resource/control/skip_next_hover.svg", ":/resource/control/skip_next_press.svg");

	sound_mute_btn_stack_ = new QStackedWidget();
	sound_btn_ = new YKIconButton();
	sound_btn_->Init(QSize(26, 26), ":/resource/control/volume_normal.svg",
		":/resource/control/volume_hover.svg", ":/resource/control/volume_press.svg");

	mute_btn_ = new YKIconButton();
	mute_btn_->Init(QSize(26, 26), ":/resource/control/volume_off_normal.svg",
		":/resource/control/volume_off_hover.svg", ":/resource/control/volume_off_press.svg");

	sound_mute_btn_stack_->addWidget(sound_btn_);
	sound_mute_btn_stack_->addWidget(mute_btn_);
	sound_mute_btn_stack_->setFixedSize(sound_btn_->size());

	voice_progressbar_ = new YKProgressBar();
	voice_progressbar_->setRange(0, 100);
	voice_progressbar_->setFixedSize(80, 8);
	voice_progressbar_->setOrientation(Qt::Horizontal); //不设置这一项，样式表无效
	voice_progressbar_->setStyleSheet(QString::fromStdString(KVoiceSliderCss));
	voice_progressbar_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	voice_progressbar_->setValue(100);

	fullscreen_stack_ = new QStackedWidget();
	fullscreen_btn_ = new YKIconButton();
	fullscreen_btn_->Init(QSize(26, 26), ":/resource/control/fullscreen_normal.svg",
		":/resource/control/fullscreen_hover.svg", ":/resource/control/fullscreen_press.svg");

	exit_fullscreen_btn_ = new YKIconButton();
	exit_fullscreen_btn_->Init(QSize(26, 26), ":/resource/control/fullscreen_exit_normal.svg",
		":/resource/control/fullscreen_exit_hover.svg", ":/resource/control/fullscreen_exit_press.svg");

	fullscreen_stack_->addWidget(fullscreen_btn_);
	fullscreen_stack_->addWidget(exit_fullscreen_btn_);
	fullscreen_stack_->setFixedSize(fullscreen_btn_->size());

	control_hbox_layout->addSpacing(20);
	control_hbox_layout->addWidget(pos_dur_widget_);
	control_hbox_layout->addSpacing(30);
	control_hbox_layout->addWidget(stop_btn_);
	control_hbox_layout->addSpacing(30);
	control_hbox_layout->addWidget(previous_btn_);
	control_hbox_layout->addSpacing(30);
	control_hbox_layout->addWidget(start_pause_btn_stack_);
	control_hbox_layout->addSpacing(30);
	control_hbox_layout->addWidget(next_btn_);
	control_hbox_layout->addSpacing(30);
	control_hbox_layout->addWidget(sound_mute_btn_stack_);
	control_hbox_layout->addSpacing(12);
	control_hbox_layout->addWidget(voice_progressbar_);
	control_hbox_layout->addSpacing(12);
	control_hbox_layout->addWidget(fullscreen_stack_);
	control_hbox_layout->addStretch(1);

	main_vbox_layout->addSpacing(6);
	main_vbox_layout->addLayout(control_hbox_layout);
	main_vbox_layout->addSpacing(6);

	Restore();
}

void PlayControlWidget::InitSignalChannels() {
	connect(stop_btn_, &QPushButton::clicked, this, [=]() {
		AppStopPlayMsg msg{};
		context_->SendAppMessage(msg);
		Restore();
	});
	connect(previous_btn_, &QPushButton::clicked, this, [=]() {
		Q_EMIT SigPrevious();
	});
	connect(pause_btn_, &QPushButton::clicked, this, [=]() {
		AppPausePlayMsg msg{};
		context_->SendAppMessage(msg);
	});
	connect(start_btn_, &QPushButton::clicked, this, [=]() {
		AppResumePlayMsg msg{};
		context_->SendAppMessage(msg);
	});
	connect(next_btn_, &QPushButton::clicked, this, [=]() {
		Q_EMIT SigNext();
	});
	connect(mute_btn_, &QPushButton::clicked, this, [=]() {
		AppSetUnmuteMsg msg{};
		context_->SendAppMessage(msg);
	});
	connect(sound_btn_, &QPushButton::clicked, this, [=]() {
		AppSetMuteMsg msg{};
		context_->SendAppMessage(msg);
	});

	connect(fullscreen_btn_, &QPushButton::clicked, this, [=]() {
		AppFullScreenMsg msg{};
		context_->SendAppMessage(msg);
	});

	connect(exit_fullscreen_btn_, &QPushButton::clicked, this, [=]() {
		AppExitFullScreenMsg msg{};
		context_->SendAppMessage(msg);
	});

	connect(play_progress_bar_, &YKProgressBar::SigPosChanged, this, [=]() {
		progress_seeking_ = true;
		seek_pos_ = play_progress_bar_->value();
		AppSeekPosMsg msg{.pos = seek_pos_};
		context_->SendAppMessage(msg);
		QTimer::singleShot(1000, [=]() {
			progress_seeking_ = false;
		});
	});

	connect(voice_progressbar_, &YKProgressBar::SigPosChanged, this, [=]() {
		auto volume = voice_progressbar_->value();
		AppSetVolumeMsg msg{ .volume = volume };
		context_->SendAppMessage(msg);
	});

}

void PlayControlWidget::RegisterEvents() {
	if (!context_) {
		return;
	}

	msg_listener_->Listen<AppGotDurationMsg>([=, this](const AppGotDurationMsg& event) {
		context_->PostUITask([=, this]() {
			auto duration_str = GetFormatTimeString(event.duration);
			if (pos_dur_widget_->isHidden()) {
				pos_dur_widget_->show();
			}
			duration_lab_->setText(duration_str);
			play_progress_bar_->setEnabled(true);
			play_progress_bar_->setRange(0, event.duration);
		});
	});

	msg_listener_->Listen<AppLibvlcMediaPlayerEncounteredErrorMsg>([=, this](const AppLibvlcMediaPlayerEncounteredErrorMsg& event) {
		context_->PostUITask([=, this]() {
			Restore();
		});
	});

	msg_listener_->Listen<AppLibvlcMediaPlayerStoppedMsg>([=, this](const AppLibvlcMediaPlayerStoppedMsg& event) {
		context_->PostUITask([=, this]() {
			Restore();
		});
	});

	msg_listener_->Listen<AppLibvlcMediaPlayerEndReachedMsg>([=, this](const AppLibvlcMediaPlayerEndReachedMsg& event) {
		context_->PostUITask([=, this]() {
			Restore();
		});
	});

	msg_listener_->Listen<AppLibvlcMediaPlayerPlayingMsg>([=, this](const AppLibvlcMediaPlayerPlayingMsg& event) {
		context_->PostUITask([=, this]() {
			start_pause_btn_stack_->setCurrentWidget(pause_btn_);
			AppMainWindowResizeMsg msg{};
			context_->SendAppMessage(msg);
			if (has_handle_playing_) {
				return;
			}
			has_handle_playing_ = true;
			voice_progressbar_->setEnabled(true);
			pos_dur_widget_->show();
			fullscreen_btn_->setEnabled(true);
			exit_fullscreen_btn_->setEnabled(true);
			if (sound_mute_btn_stack_->currentWidget() == mute_btn_) {
				AppSetMuteMsg msg{};
				context_->SendAppMessage(msg);
			}
			else {
				auto volume = voice_progressbar_->value();
				AppSetVolumeMsg msg{ .volume = volume };
				context_->SendAppMessage(msg);
			}
		});
	});

	msg_listener_->Listen<AppLibvlcMediaPlayerPausedMsg>([=, this](const AppLibvlcMediaPlayerPausedMsg& event) {
		context_->PostUITask([=, this]() {
			start_pause_btn_stack_->setCurrentWidget(start_btn_);
		});
	});

	msg_listener_->Listen<AppLibvlcMediaPlayerTimeChangedMsg>([=, this](const AppLibvlcMediaPlayerTimeChangedMsg& event) {
		context_->PostUITask([=, this]() {
			auto current_movie_time = GetFormatTimeString(event.current_movie_time);
			pos_lab_->setText(current_movie_time);
			if (!progress_seeking_) {
				play_progress_bar_->setValue(event.current_movie_time);
			}
		});
	});

	msg_listener_->Listen<AppLibvlcMediaPlayerMutedMsg>([=, this](const AppLibvlcMediaPlayerMutedMsg& event) {
		context_->PostUITask([=, this]() {
			sound_mute_btn_stack_->setCurrentWidget(mute_btn_);
		});
	});

	msg_listener_->Listen<AppLibvlcMediaPlayerUnmutedMsg>([=, this](const AppLibvlcMediaPlayerUnmutedMsg& event) {
		context_->PostUITask([=, this]() {
			sound_mute_btn_stack_->setCurrentWidget(sound_btn_);
		});
	});

	msg_listener_->Listen<AppFullScreenTakeEffectMsg>([=, this](const AppFullScreenTakeEffectMsg& event) {
		context_->PostUITask([=, this]() {
			fullscreen_stack_->setCurrentWidget(exit_fullscreen_btn_);
		});
	});

	msg_listener_->Listen<AppExitFullScreenTakeEffectMsg>([=, this](const AppExitFullScreenTakeEffectMsg& event) {
		context_->PostUITask([=, this]() {
			fullscreen_stack_->setCurrentWidget(fullscreen_btn_);
		});
	});

	fullscreen_btn_->installEventFilter(this);
	exit_fullscreen_btn_->installEventFilter(this);
}

void PlayControlWidget::Restore() {
	has_handle_playing_ = false;
	seek_pos_ = 0;
	voice_progressbar_->setEnabled(false);
	play_progress_bar_->setEnabled(false);
	play_progress_bar_->setRange(0, 0);
	pos_dur_widget_->hide();
	start_pause_btn_stack_->setCurrentWidget(start_btn_);
	AppExitFullScreenMsg msg{};
	context_->SendAppMessage(msg);
	fullscreen_btn_->setEnabled(false);
	exit_fullscreen_btn_->setEnabled(false);
}

QString PlayControlWidget::GetFormatTimeString(int ms) {
	int hour = ms / 1000 / 60 / 60;
	int minute = ms / 1000 / 60 % 60;
	int second = ms / 1000 % 60;
	return QString("%1:%2:%3").arg(hour).arg(minute, 2, 10, QLatin1Char('0')).arg(second, 2, 10, QLatin1Char('0'));
}

bool PlayControlWidget::eventFilter(QObject* obj, QEvent* event) {
	if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
		QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
		if (keyEvent->key() == Qt::Key_Space) {
			if (obj == fullscreen_btn_ || obj == exit_fullscreen_btn_) {
				return true; // 阻止事件继续传播
			}		
		}
	}
	return QWidget::eventFilter(obj, event);
}

}