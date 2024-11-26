#pragma once
#include <qwidget.h>
#include <memory>
class QStackedWidget;

namespace yk {

class Context;
class MessageListener;
class PlayView;
class PlayBeforeWidget;

class PlayWidget : public QWidget{
	Q_OBJECT
public:
	PlayWidget(const std::shared_ptr<Context>& context, QWidget* parent = nullptr);
	~PlayWidget();
	
private:
	void InitView();
	void InitSigChannels();
	void RegisterEvents();

	QStackedWidget* stacked_widget_ = nullptr;
	PlayView* play_view_ = nullptr;
	PlayBeforeWidget* play_before_widget_ = nullptr;

private:
	std::shared_ptr<Context> context_ = nullptr;;
	std::shared_ptr<MessageListener> msg_listener_ = nullptr;
};


}