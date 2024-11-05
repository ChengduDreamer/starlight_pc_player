#include "play_widget.h"

#include <qsizepolicy.h>

namespace yk {

PlayWidget::PlayWidget(QWidget* parent) : BasePage(parent) {
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	//setFixedSize(400, 400);
	//setStyleSheet("QWidget {background-color: #000000;}");
}


PlayWidget::~PlayWidget() {

}

}