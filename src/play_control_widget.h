#pragma once
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class QStackedWidget;
class YKProgressBar;
class YKIconButton;

namespace yk {

class PlayControlWidget : public QWidget {
public:
	PlayControlWidget(QWidget* parent = nullptr);
	~PlayControlWidget();

private:
	void InitView();

	YKProgressBar* play_progress_bar_ = nullptr;

	QLabel* duration_lap_ = nullptr;
	YKIconButton* stop_btn_ = nullptr;
	YKIconButton* previous_btn_ = nullptr;
	QStackedWidget* start_pause_btn_stack_ = nullptr;
	YKIconButton* start_btn_ = nullptr;
	YKIconButton* pause_btn_ = nullptr;
	YKIconButton* next_btn_ = nullptr;

	QStackedWidget* sound_mute_btn_stack_ = nullptr;
	YKIconButton* mute_btn_ = nullptr;
	YKIconButton* sound_btn_ = nullptr;
	YKProgressBar* voice_progressbar_ = nullptr;

	QStackedWidget* fullscreen_stack_ = nullptr;
	YKIconButton* fullscreen_btn_ = nullptr;
	YKIconButton* exit_fullscreen_btn_ = nullptr;
};
}