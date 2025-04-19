#pragma once
#include <qwidget.h>

class QHBoxLayout;
class YKIconButton;

namespace yk {

class ListFuncPage : public QWidget {
	Q_OBJECT
public:
	ListFuncPage(QWidget* parent = nullptr);
    ~ListFuncPage();
private:
    void InitView();

private:
	QHBoxLayout* hbox_main_layout_ = nullptr;
	YKIconButton* clear_history_btn_ = nullptr;
	YKIconButton* add_file_btn_ = nullptr;
};


}

