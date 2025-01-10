#include "image_panel.h"
#include <qboxlayout.h>
#include <qdebug.h>
#include <qtimer.h>
#include "app_messages.h"
#include "context.h"
//#include "image_view.h"
//#include "image_toolbar.h"
//#include "image_container.h"

#include "draw_board/draw_board_widget.h"
#include "draw_board/draw_widget.h"

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

	/*image_container_ = new ImageContainer(context_, this);
	main_vbox_layout->addWidget(image_container_);

	toolbar_ = new ImageToolbar(context_, this);
	main_vbox_layout->addWidget(toolbar_); */

	draw_board_widget_ = new DrawBoardWidget();
	main_vbox_layout->addWidget(draw_board_widget_);
}

void ImagePanel::RegisterEvents() {

	msg_listener_->Listen<AppCaptureImageCompletedMsg>([=, this](const AppCaptureImageCompletedMsg& event) {
		context_->PostUITask([=, this]() {
			qDebug() << "ImagePanel AppCaptureImageCompletedMsg";
			if (!event.success) {
				qDebug() << "ImagePanel AppCaptureImageCompletedMsg error";
				return;
			}
			showMaximized();
			auto p = event.pixmap;
			draw_board_widget_->GetDrawWidget()->Clear();
			draw_board_widget_->GetDrawWidget()->SetBackground(std::move(p));
			
		});
	});
}

}