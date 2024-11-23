#pragma once
#include <qmainwindow.h>

class QHBoxLayout;
class QVBoxLayout;

namespace yk {
	
class BasePage;
class PlayWidget;
class PlayControlWidget;
class PlayListWidget;

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();
	void InitView();

private:
	QWidget* bg_page_ = nullptr;

	QHBoxLayout* main_hbox_layout_ = nullptr;

	QVBoxLayout* play_vbox_layout_ = nullptr;
	PlayWidget* play_widget_ = nullptr;
	PlayControlWidget* play_control_widget_ = nullptr;

	QVBoxLayout* list_vbox_layout_ = nullptr;
	PlayListWidget* list_widget_ = nullptr;
};


}