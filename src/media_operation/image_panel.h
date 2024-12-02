#pragma once
#include <memory>
#include <qwidget.h>

namespace yk {
class ImageToolbar;
class ImageView;
class Context;
class MessageListener;

class ImagePanel : public QWidget {
public:
	ImagePanel(const std::shared_ptr<Context>& context, QWidget* parent = nullptr);
	~ImagePanel();
private:
	void InitView();
	void RegisterEvents();
private:
	ImageView* image_view_ = nullptr;
	ImageToolbar* toolbar_ = nullptr;
private:
	std::shared_ptr<Context> context_ = nullptr;
	std::shared_ptr<MessageListener> msg_listener_ = nullptr;
};

}