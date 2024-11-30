#pragma once

#include <qwidget.h>

namespace yk {

class BgPage : public QWidget {
	Q_OBJECT
public:
	BgPage(QWidget* parent = nullptr);
	void mouseMoveEvent(QMouseEvent* event) override;

};


}