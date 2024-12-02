#pragma once
#include <memory>
#include <qwidget.h>
#include <qpixmap.h>
#include <qevent.h>
#include <qpainter.h>
namespace yk {

class Context;
class MessageListener;

class ImageView : public QWidget {
public:
	static std::shared_ptr<ImageView> Make(const std::shared_ptr<Context>& context, QWidget* parent = nullptr);
	ImageView(const std::shared_ptr<Context>& context, QWidget* parent = nullptr);
	~ImageView();
	void paintEvent(QPaintEvent* event) override;
	void SetPixmap(const QPixmap pixmap);
private:
	QPixmap pixmap;

private:
	std::shared_ptr<Context> context_ = nullptr;
	std::shared_ptr<MessageListener> msg_listener_ = nullptr;
};

}