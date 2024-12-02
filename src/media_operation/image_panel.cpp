#include "image_panel.h"
#include <qboxlayout.h>
#include <qdebug.h>
#include "app_messages.h"
#include "context.h"
#include "image_view.h"
#include "image_toolbar.h"

namespace yk {

ImagePanel::ImagePanel(const std::shared_ptr<Context>& context, QWidget* parent) : context_(context), QWidget(parent) {
	msg_listener_ = context->CreateMessageListener();
	setAttribute(Qt::WA_StyledBackground);
	InitView();
	RegisterEvents();
}

ImagePanel::~ImagePanel() {

}

void ImagePanel::InitView() {
	resize(800, 600);
	QVBoxLayout* main_vbox_layout = new QVBoxLayout(this);
	main_vbox_layout->setAlignment(Qt::AlignTop);
	main_vbox_layout->setSpacing(0);
	main_vbox_layout->setContentsMargins(0, 0, 0, 0);

	image_view_ = new ImageView(context_, this);
	main_vbox_layout->addWidget(image_view_);

	toolbar_ = new ImageToolbar(context_, this);
	main_vbox_layout->addWidget(toolbar_); 
}

void ImagePanel::RegisterEvents() {

	msg_listener_->Listen<AppCaptureImageCompletedMsg>([=, this](const AppCaptureImageCompletedMsg& event) {
		context_->PostUITask([=, this]() {
			qDebug() << "ImagePanel AppCaptureImageCompletedMsg";
			if (!event.success) {
				qDebug() << "ImagePanel AppCaptureImageCompletedMsg error";
				return;
			}
			image_view_->SetPixmap(event.pixmap);
			
			resize(event.pixmap.width(), event.pixmap.height() + toolbar_->height());
		});
	});
}

}