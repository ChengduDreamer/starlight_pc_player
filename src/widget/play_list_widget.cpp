#include "play_list_widget.h"

namespace yk {

PlayListWidget::PlayListWidget(QWidget* parent) : QWidget(parent) {
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	//setFixedWidth(180);
	setStyleSheet("QWidget {background-color: #000000;}");
	setMinimumWidth(180);
}

PlayListWidget::~PlayListWidget() {

}

}