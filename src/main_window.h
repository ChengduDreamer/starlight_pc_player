#pragma once

#include "ElaWindow.h"

namespace sp {
	
class MainWindow : public ElaWindow {
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();
	void InitView();

};


}