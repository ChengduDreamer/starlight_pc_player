#include "image_container.h"
#include <qsizepolicy.h>
#include <qboxlayout.h>
#include "image_view.h"
#include "cpp_base_lib/yk_logger.h"
namespace yk {

ImageContainer::ImageContainer(const std::shared_ptr<Context>& context, QWidget* parent) : context_(context), QWidget(parent) {
	setAttribute(Qt::WA_StyledBackground);
	setStyleSheet("QWidget {background-color: #ffffff;}");
	auto size_policy = this->sizePolicy();
	size_policy.setHorizontalPolicy(QSizePolicy::Expanding);
	size_policy.setVerticalPolicy(QSizePolicy::Expanding);
	setSizePolicy(size_policy);

	auto main_hbox_layout = new QHBoxLayout(this);
	main_hbox_layout->setSpacing(0);
	main_hbox_layout->setContentsMargins(0, 0, 0, 0);
	main_hbox_layout->setAlignment(Qt::AlignLeft);

	image_view_ = new ImageView(context_, this);
	main_hbox_layout->addWidget(image_view_, 5);

	whiteboard_ = new QWidget();
	whiteboard_->setStyleSheet("QWidget {background-color: #ffffff;}");
	main_hbox_layout->addWidget(whiteboard_, 1);
}

ImageContainer::~ImageContainer() {
	YK_LOGI("~ImageView");
}

void ImageContainer::SetPixmap(QPixmap&& pixmap) {
	image_view_->SetPixmap(std::move(pixmap));
}

}