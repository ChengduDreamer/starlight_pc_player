#include "media_operation.h"
#include <qdatetime.h>
#include <qpixmap.h>
#include <qstandardpaths.h>
#include <qwidget.h>
#include "context.h"
#include "app_messages.h"

namespace yk {

std::shared_ptr<MediaOperation> MediaOperation::Make(const std::shared_ptr<Context>& context) {
	return std::make_shared<MediaOperation>(context);
}

MediaOperation::MediaOperation(const std::shared_ptr<Context>& context) : context_(context) {
	msg_listener_ = context_->CreateMessageListener();
	RegisterEvents();
}

MediaOperation::~MediaOperation() {

}

void MediaOperation::RegisterEvents() {
	//msg_listener_->Listen<AppCaptureImageMsg>([=, this](const AppCaptureImageMsg& event) {
	//	context_->PostUITask([=, this]() {
	//		CaptureImage(event.widget_ptr);
	//	});
	//});
}


bool MediaOperation::CaptureImage(QWidget* target) {
	if (!target) {
		return false;
	}
	static int count = -1;
	QString png_name = QString::number(QDateTime::currentMSecsSinceEpoch()) + QString("_") + QString::number(++count) + ".png";

	QString pic_path = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);

	QPixmap pixmap = QPixmap::grabWindow(target->winId()); // 其他方法无效,比如grab(), 无法采集libvlc渲染画面 因为可能libvlc启用了硬件加速

	AppCaptureImageCompletedMsg msg{.pixmap = pixmap};
	context_->SendAppMessage(msg);
	
	if (!pixmap.save(pic_path + "/" + png_name, "png")) {
		return false;
	}
	return true;
}

}