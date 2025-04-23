#pragma once
#include <qevent.h>
#include <qdialog.h>
class QWidget;
class QPushButton;
class QLabel;
class QHBoxLayout;
class QVBoxLayout;
class QTimer;

namespace yk {
//弹窗
class PopupWidget : public QDialog {
	Q_OBJECT
public:
	
	PopupWidget(QWidget* parent = nullptr);
	void Init();
	void SetData(const QString& title, const QString& content);
	void DelayClose(int ms);

	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
private:
	QVBoxLayout* main_vbox_layout_ = nullptr;
	// 标题、关闭按钮
	QHBoxLayout* title_hbox_layout_ = nullptr;
	QLabel* title_lab_ = nullptr;
	QPushButton* close_btn_ = nullptr;

	// 远端文件信息
	QHBoxLayout* content_layout_ = nullptr;
	QLabel* content_lab_ = nullptr;
	
	// 相关操作
	QHBoxLayout* operation_hbox_layout_ = nullptr;
	QPushButton* ok_btn_ = nullptr;
	QPushButton* cancel_btn_ = nullptr;

	bool pressed_ = false;
	QPoint point_;

private:
	void InitSigChannel();
};

}