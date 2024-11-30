#include "bg_page.h"

#include <qdebug.h>

namespace yk {

BgPage::BgPage(QWidget* parent) : QWidget(parent) {
}

void BgPage::mouseMoveEvent(QMouseEvent* event) {
	return QWidget::mouseMoveEvent(event);
}

}