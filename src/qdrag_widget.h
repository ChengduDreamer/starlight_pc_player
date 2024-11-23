#pragma once

#include <qwidget.h>
#include <qevent.h>

namespace yk {

class QDragWidget : public QWidget {
	Q_OBJECT
public:
	QDragWidget(QWidget* parent = NULL);
	~QDragWidget();
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dropEvent(QDropEvent* event) override;

signals:
	void SigOpenUrl(QString url);
};

}