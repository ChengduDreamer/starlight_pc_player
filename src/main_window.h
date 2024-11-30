#pragma once
#include <memory>
#include <qmainwindow.h>
#include <qpainter.h>
#include <qevent.h>
#include <qobject.h>
#include <qwidget.h>
class QHBoxLayout;
class QVBoxLayout;
class QTimer;

namespace QWK {
class WidgetWindowAgent;
class WindowBar;
}


namespace yk {
	
class Context;
class MessageListener;
class BasePage;
class PlayWidget;
class PlayControlWidget;
class PlayListWidget;
class BgPage;

class MainWindow : public /*QMainWindow*/ QWidget {
	Q_OBJECT

public:
	MainWindow(const std::shared_ptr<Context>& context, QWidget* parent = nullptr);
	~MainWindow();
	
	void mouseMoveEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
	bool eventFilter(QObject* obj, QEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;

	enum Theme {
		Dark,
		Light,
	};
	//Q_ENUM(Theme)
Q_SIGNALS:
	void themeChanged();
private:
	void InstallWindowAgent();
	void InitView();
	void InitTitlebar();
	void RegisterEvents();
	void InitTimer();

	//void loadStyleSheet(Theme theme);

	//Theme currentTheme{};

private:
	QWK::WindowBar* window_bar_ = nullptr;
	BgPage* bg_page_ = nullptr;

	QHBoxLayout* main_hbox_layout_ = nullptr;

	QVBoxLayout* play_vbox_layout_ = nullptr;
	PlayWidget* play_widget_ = nullptr;
	PlayControlWidget* play_control_widget_ = nullptr;

	QVBoxLayout* list_vbox_layout_ = nullptr;
	PlayListWidget* list_widget_ = nullptr;

	QWK::WidgetWindowAgent* windowAgent = nullptr;

	QTimer* timer_ = nullptr;
private:

	std::shared_ptr<Context> context_ = nullptr;
	std::shared_ptr<MessageListener> msg_listener_ = nullptr;
};


}