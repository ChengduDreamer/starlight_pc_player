#pragma once

#include "ElaWindow.h"

class QHBoxLayout;
class QVBoxLayout;

namespace yk {
	
class PlayWidget;

class MainWindow : public ElaWindow {
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();
	void InitView();

private:
	QVBoxLayout* vbox_play_layout_ = nullptr;
	PlayWidget* play_widget_ = nullptr;
};


}