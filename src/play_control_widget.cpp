#include "play_control_widget.h"
#include <qsizepolicy.h>

namespace yk {

PlayControlWidget::PlayControlWidget(QWidget* parent) : QWidget(parent) {
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	//setFixedSize(400, 400);
	//setStyleSheet("QWidget {background-color: #000000;}");

	//setFixedHeight(120);
	setStyleSheet("QWidget {background-color: #00ff00;}");
}


PlayControlWidget::~PlayControlWidget() {

}

}