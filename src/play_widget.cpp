#include "play_widget.h"
#include <qsizepolicy.h>

namespace yk {

PlayWidget::PlayWidget(QWidget* parent) : QWidget(parent) {
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setStyleSheet("QWidget {background-color: #ff0000;}");
}


PlayWidget::~PlayWidget() {

}

}