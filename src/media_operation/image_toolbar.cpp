#include "image_toolbar.h"
#include <qsizepolicy.h>
#include "cpp_base_lib/yk_logger.h"
namespace yk {

ImageToolbar::ImageToolbar(const std::shared_ptr<Context>& context, QWidget* parent) : context_(context), QWidget(parent) {
	setAttribute(Qt::WA_StyledBackground);
	/*setAttribute(Qt::WA_TranslucentBackground);*/

	setStyleSheet("QWidget {background-color: #0000ff;}");
	auto size_policy = this->sizePolicy();
	size_policy.setHorizontalPolicy(QSizePolicy::Expanding);
	setFixedHeight(40);
}

ImageToolbar::~ImageToolbar() {
	YK_LOGI("~ImageView");
}


}