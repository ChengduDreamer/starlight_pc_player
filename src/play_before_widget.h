#pragma once

#include "qdrag_widget.h"
class YKButton;
class YKLineEdit;

namespace yk {

class PlayBeforeWidget : public QDragWidget {
	Q_OBJECT
public:
	PlayBeforeWidget(QWidget* parent = nullptr);
	~PlayBeforeWidget();
private:
	void InitView();
	void InitSigChannel();
private:
	YKButton* open_file_btn_ = nullptr;
	YKLineEdit* url_edit_ = nullptr;
	YKButton* open_url_btn_ = nullptr;
};

}