#pragma once
#include <memory>
#include <qobject.h>

class QWidget;

namespace yk {

class Context;
class MessageListener;

class MediaOperation  {
public:
	static std::shared_ptr<MediaOperation> Make(const std::shared_ptr<Context>& context);
	MediaOperation(const std::shared_ptr<Context>& context);
	~MediaOperation();
private:
	void RegisterEvents();
	bool CaptureImage(QWidget* target);
private:
	std::shared_ptr<Context> context_ = nullptr;
	std::shared_ptr<MessageListener> msg_listener_ = nullptr;
};

}