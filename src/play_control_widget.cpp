#include "play_control_widget.h"
#include <qsizepolicy.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <qboxlayout.h>
#include <qpixmap.h>

#include "public/yk_icon_button.h"
#include "public/yk_progress_bar.h"
#include "public/yk_style_sheet.h"

namespace yk {

PlayControlWidget::PlayControlWidget(QWidget* parent) : QWidget(parent) {
	InitView();
	InitSignalChannels();
}

PlayControlWidget::~PlayControlWidget() {

}

void PlayControlWidget::InitView() {
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setAttribute(Qt::WA_StyledBackground);
	setStyleSheet("QWidget {background-color: #000000;}");

	QVBoxLayout* main_vbox_layout = new QVBoxLayout(this);
	main_vbox_layout->setAlignment(Qt::AlignTop);
	main_vbox_layout->setContentsMargins(0,0,0,0);
	main_vbox_layout->setSpacing(0);
	
	play_progress_bar_ = new YKProgressBar();
	play_progress_bar_->setFixedHeight(14);
	play_progress_bar_->setOrientation(Qt::Horizontal); //不设置这一项，样式表无效
	play_progress_bar_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	play_progress_bar_->setAttribute(Qt::WA_StyledBackground);
	play_progress_bar_->setStyleSheet(QString::fromStdString(KPlayerSliderCss));

	main_vbox_layout->addSpacing(6);
	main_vbox_layout->addWidget(play_progress_bar_);

	QHBoxLayout* control_hbox_layout = new QHBoxLayout();
	control_hbox_layout->setContentsMargins(0,0,0,0);
	control_hbox_layout->setAlignment(Qt::AlignLeft);
	control_hbox_layout->setSpacing(0);

	duration_lap_ = new QLabel();
	duration_lap_->setText("00:00:00 / 00:00:00");
	duration_lap_->setStyleSheet("QLabel {background-color: transparent; font-family:Microsoft YaHei;font-size:12px; color:#ffffff;}");

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
	start_pause_btn_stack_->setFixedSize(pause_btn_->size());

	next_btn_ = new YKIconButton();
	next_btn_->Init(QSize(26, 26), ":/resource/control/skip_next_normal.svg",
		":/resource/control/skip_next_hover.svg", ":/resource/control/skip_next_press.svg");

	sound_mute_btn_stack_ = new QStackedWidget();
	mute_btn_ = new YKIconButton();
	mute_btn_->Init(QSize(26, 26), ":/resource/control/volume_normal.svg",
		":/resource/control/volume_hover.svg", ":/resource/control/volume_press.svg");

	sound_btn_ = new YKIconButton();
	sound_btn_->Init(QSize(26, 26), ":/resource/control/volume_off_normal.svg",
		":/resource/control/volume_off_hover.svg", ":/resource/control/volume_off_press.svg");

	sound_mute_btn_stack_->addWidget(mute_btn_);
	sound_mute_btn_stack_->addWidget(sound_btn_);
	sound_mute_btn_stack_->setFixedSize(sound_btn_->size());

	voice_progressbar_ = new YKProgressBar();
	voice_progressbar_->setRange(0, 100);
	voice_progressbar_->setFixedSize(80, 8);
	voice_progressbar_->setOrientation(Qt::Horizontal); //不设置这一项，样式表无效
	voice_progressbar_->setStyleSheet(QString::fromStdString(KVoiceSliderCss));
	voice_progressbar_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

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
	control_hbox_layout->addWidget(duration_lap_);
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
	control_hbox_layout->addStretch(1);
	control_hbox_layout->addWidget(fullscreen_stack_);
	control_hbox_layout->addStretch(1);
	control_hbox_layout->addSpacing(30);

	main_vbox_layout->addSpacing(6);
	main_vbox_layout->addLayout(control_hbox_layout);
}

void PlayControlWidget::InitSignalChannels() {
	connect(stop_btn_, &QPushButton::clicked, this, [=]() {
		Q_EMIT SigStop();
	});
	connect(previous_btn_, &QPushButton::clicked, this, [=]() {
		Q_EMIT SigPrevious();
	});
	connect(pause_btn_, &QPushButton::clicked, this, [=]() {
		Q_EMIT SigPause();
	});
	connect(start_btn_, &QPushButton::clicked, this, [=]() {
		Q_EMIT SigPlay();
	});
	connect(next_btn_, &QPushButton::clicked, this, [=]() {
		Q_EMIT SigNext();
	});
	connect(mute_btn_, &QPushButton::clicked, this, [=]() {
		Q_EMIT SigMute();
	});
	connect(sound_btn_, &QPushButton::clicked, this, [=]() {
		Q_EMIT SigSound();
	});
	connect(fullscreen_btn_, &QPushButton::clicked, this, [=]() {
		Q_EMIT SigFullScreen();
	});
	connect(exit_fullscreen_btn_, &QPushButton::clicked, this, [=]() {
		Q_EMIT SigExitFullScreen();
	});
}

}