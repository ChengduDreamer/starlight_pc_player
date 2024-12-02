#pragma once
#include <memory>
#include <qwidget.h>
#include <qpixmap.h>
#include <qevent.h>
#include <qpainter.h>
namespace yk {

class Context;
class MessageListener;

class ImageToolbar : public QWidget {
public:
	ImageToolbar(const std::shared_ptr<Context>& context, QWidget* parent = nullptr);
	~ImageToolbar();
private:
	std::shared_ptr<Context> context_ = nullptr;
	std::shared_ptr<MessageListener> msg_listener_ = nullptr;
};

}