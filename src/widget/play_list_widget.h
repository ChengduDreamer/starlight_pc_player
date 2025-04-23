#pragma once
#include <qwidget.h>
#include <memory>

class QVBoxLayout;
class QHBoxLayout;
class YKIconButton;

namespace yk {

class Context;
class MessageListener;
class MediaItemWidget;
class MediaListWidget;
class ListFuncPage;

class PlayListWidget : public QWidget {
public:
	PlayListWidget(std::shared_ptr<Context> context, QWidget* parent = nullptr);
	~PlayListWidget();
private:
	void InitView();
private:
	QVBoxLayout* vbox_main_layout_ = nullptr;
    MediaListWidget* media_list_widget_ = nullptr;
	ListFuncPage* list_func_page_ = nullptr;

	std::shared_ptr<Context> context_ = nullptr;;
	std::shared_ptr<MessageListener> msg_listener_ = nullptr;

};

}