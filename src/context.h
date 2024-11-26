#pragma once

#include <memory>
#include <QObject>
#include <qwindowdefs.h>
#include "asio2/asio2.hpp"
#include "cpp_base_lib/thread.h"
#include "cpp_base_lib/msg_notifier.h"


namespace yk {

class Settings;
class MessageNotifier;

class Context : public std::enable_shared_from_this<Context>, public QObject {
public:
	static std::shared_ptr<Context> Make();
	Context();
	bool Init();
	void Exit();
	void PostTask(std::function<void()>&& task);
	void PostUITask(std::function<void()>&& task);
	Settings* GetSettings();

	template<typename T>
	void SendAppMessage(const T& m) {
		if (msg_thread_ && msg_notifier_) {
			msg_thread_->Post([=, this]() {
				msg_notifier_->SendAppMessage(m);
			});
		}
	}
	std::shared_ptr<MessageListener> CreateMessageListener();
	std::shared_ptr<MessageNotifier> GetMessageNotifier();
private:
	void InitTimers();

private:
	Settings* settings_ = nullptr;
	std::shared_ptr<Thread> msg_thread_ = nullptr;
	std::shared_ptr<MessageNotifier> msg_notifier_ = nullptr;

	std::shared_ptr<asio2::iopool> iopool_ = nullptr;
	std::shared_ptr<asio2::iopool> net_iopool_ = nullptr;
	std::shared_ptr<asio2::timer> timer_ = nullptr;
};


}