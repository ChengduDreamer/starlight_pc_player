#pragma once
#include <qwidget.h>

class QStackedWidget;

namespace yk {

class PlayView;

class PlayWidget : public QWidget{
public:
	PlayWidget(QWidget* parent = nullptr);
	~PlayWidget();
	
private:
	void InitView();

	QStackedWidget* stacked_widget_ = nullptr;
	PlayView* play_view_ = nullptr;
	
};


}