#pragma once

#include <qwidget.h>
#include <qevent.h>

namespace yk {

class Context;

class QDragWidget : public QWidget {
	Q_OBJECT
public:
	QDragWidget(const std::shared_ptr<Context>& context, QWidget* parent = NULL);
	~QDragWidget();
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dropEvent(QDropEvent* event) override;
	bool eventFilter(QObject* obj, QEvent* event) override;
protected:
	std::shared_ptr<Context> context_ = nullptr;
};

}