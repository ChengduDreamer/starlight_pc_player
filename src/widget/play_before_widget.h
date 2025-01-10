#pragma once
#include <qevent.h>
#include "qdrag_widget.h"
class YKButton;
class YKLineEdit;
class QSettings;
namespace yk {

class Context;

class PlayBeforeWidget : public QDragWidget {
	Q_OBJECT
public:
	PlayBeforeWidget(const std::shared_ptr<Context>& context, QWidget* parent = nullptr);
	bool eventFilter(QObject* obj, QEvent* event) override;
	~PlayBeforeWidget();
private:
	void InitView();
	void InitSigChannel();
private:
	YKButton* open_file_dialog_btn_ = nullptr;
	YKLineEdit* url_edit_ = nullptr;
	YKButton* open_url_btn_ = nullptr;

private:
private:
	std::shared_ptr<QSettings> qsettings_ = nullptr;
};

}