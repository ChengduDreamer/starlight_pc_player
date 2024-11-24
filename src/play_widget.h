#pragma once
#include <qwidget.h>

class QStackedWidget;

namespace yk {

class PlayView;
class PlayBeforeWidget;

class PlayWidget : public QWidget{
	Q_OBJECT
public:
	PlayWidget(QWidget* parent = nullptr);
	~PlayWidget();
	
private:
	void InitView();
	void InitSigChannels();

	QStackedWidget* stacked_widget_ = nullptr;
	PlayView* play_view_ = nullptr;
	PlayBeforeWidget* play_before_widget_ = nullptr;
};


}