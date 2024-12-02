#include "image_view.h"
#include <qsizepolicy.h>
#include "cpp_base_lib/yk_logger.h"
namespace yk {

std::shared_ptr<ImageView> ImageView::Make(const std::shared_ptr<Context>& context, QWidget* parent) {
	return std::make_shared<ImageView>(context, parent);
}


ImageView::ImageView(const std::shared_ptr<Context>& context, QWidget* parent) : context_(context), QWidget(parent) {
	setAttribute(Qt::WA_StyledBackground);
	/*setAttribute(Qt::WA_TranslucentBackground);*/

	setStyleSheet("QWidget {background-color: #ff0000;}");
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
}

ImageView::~ImageView() {
	YK_LOGI("~ImageView");
}

void ImageView::SetPixmap(const QPixmap p) {
	pixmap = p;
	resize(p.width(), p.height());
	update();
}

void ImageView::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setPen(Qt::NoPen);
	painter.save();
	painter.drawPixmap(0, 0, this->width(), this->height(), pixmap);
	painter.restore();
	QWidget::paintEvent(event);
}

}