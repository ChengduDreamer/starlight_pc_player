#include "play_view.h"
#include <qsizepolicy.h>

namespace yk {

PlayView::PlayView(QWidget* parent) : QWidget(parent) {
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setStyleSheet("QWidget {background-color: #ff0000;}");
}


PlayView::~PlayView() {

}

}