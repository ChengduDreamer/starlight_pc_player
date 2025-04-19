#pragma once
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class YKIconButton;

namespace yk {

class MediaItemWidget;
class MediaListWidget;
class ListFuncPage;

class PlayListWidget : public QWidget {
public:
	PlayListWidget(QWidget* parent = nullptr);
	~PlayListWidget();
private:
	void InitView();
private:
	QVBoxLayout* vbox_main_layout_ = nullptr;
    MediaListWidget* media_list_widget_ = nullptr;
	ListFuncPage* list_func_page_ = nullptr;

};

}