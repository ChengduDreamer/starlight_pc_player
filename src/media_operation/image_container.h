#pragma once

#include <qwidget.h>
#include <qpixmap.h>

namespace yk {
class ImageView;
class Context;
class MessageListener;

class ImageContainer : public QWidget {
public:
	ImageContainer(const std::shared_ptr<Context>& context, QWidget* parent = nullptr);
	~ImageContainer();
	void SetPixmap(QPixmap&& pixmap);
private:
	ImageView* image_view_ = nullptr;
	QWidget* whiteboard_ = nullptr;
private:
	std::shared_ptr<Context> context_ = nullptr;
	std::shared_ptr<MessageListener> msg_listener_ = nullptr;
};


}