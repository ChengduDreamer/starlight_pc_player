#include "popup_widget.h"
#include <mutex>
#include <iostream>
#include <qsvgrenderer.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qtimer.h>

namespace yk {

static QString s_lab_title_style = R"(
QLabel {font-size: %1px; font-family: Microsoft YaHei; color: #333333;line-height: %2px;}
)";

static QString s_lab_content_style = R"(
QLabel {font-size: %1px; font-family: Microsoft YaHei; color: #333333;line-height: %2px;}
)";

static QString s_btn_close_style = R"(
QPushButton{border:0px; background-color:#ffffff; image:url(:/resource/close_normal.svg);}
QPushButton::hover{border:0px; background-color:#bfbfbf; image:url(:/resource/close_hover.svg);} 
QPushButton::pressed{border:0px; background-color:#999999; image:url(:/resource/close_press.svg);}
)";

static QString s_btn_warning_style = R"(
QPushButton{border:0px; background-color:#ffffff; image:url(:/resource/file_exists_warning.svg);}
)";

static QString s_btn_ok_style = R"(
QPushButton{border:0px; border-radius:6px; background-color:#2979ff; font-size: %1px; font-family: Microsoft YaHei; color: #FFFFFF;}
QPushButton::hover{border:0px; border-radius:6px; background-color:#2059ee; font-size: %2px; font-family: Microsoft YaHei; color: #FFFFFF;}
QPushButton::pressed{border:0px; border-radius:6px; background-color:#1549dd; font-size: %3px; font-family: Microsoft YaHei; color: #FFFFFF;}
)";

PopupWidget::PopupWidget(QWidget* parent) :QDialog(parent) {
	Init();
	InitSigChannel();
}

void PopupWidget::DelayClose(int ms) {
	QTimer::singleShot(ms, this, [=]() {
		done(QDialog::Rejected);
	});
}

void PopupWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (pressed_) {
		move(event->pos() - point_ + pos());
	}
	QDialog::mouseMoveEvent(event);
}

void PopupWidget::mouseReleaseEvent(QMouseEvent* event)
{
	Q_UNUSED(event);
	pressed_ = false;
	QDialog::mouseReleaseEvent(event);
}

void PopupWidget::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		pressed_ = true;
		point_ = event->pos();
	}
	QDialog::mousePressEvent(event);
}

void PopupWidget::Init() {
	static std::once_flag flag;
	std::call_once(flag, []() { // init css
		s_lab_title_style = s_lab_title_style.arg(14).arg(20);
		s_lab_content_style = s_lab_content_style.arg(14).arg(20);
		s_btn_ok_style = s_btn_ok_style.arg(13).arg(13).arg(13);
		});
	setFixedSize(400, 200);
	setWindowFlags(Qt::FramelessWindowHint);
	setStyleSheet("QDialog {background-color: #ffffff; border-radius:6px; border: 1px solid #666666;}");
	setAttribute(Qt::WA_StyledBackground);

	main_vbox_layout_ = new QVBoxLayout();
	main_vbox_layout_->setContentsMargins(0,0,0,0);
	main_vbox_layout_->setSpacing(0);
	setLayout(main_vbox_layout_);
	// 标题栏 关闭按钮
	title_hbox_layout_ = new QHBoxLayout();
	title_hbox_layout_->setContentsMargins(0, 0, 0, 0);
	title_hbox_layout_->setSpacing(0);
	title_lab_ = new QLabel(this);
	title_lab_->setFixedSize(60, 20);
	title_lab_->setText(QStringLiteral(""));
	title_lab_->setStyleSheet(s_lab_title_style);

	close_btn_ = new QPushButton(this);
	close_btn_->setFixedSize(16, 16);
	close_btn_->setStyleSheet(s_btn_close_style);
	title_hbox_layout_->addSpacing(12);
	title_hbox_layout_->addWidget(title_lab_);
	title_hbox_layout_->addStretch();
	title_hbox_layout_->addWidget(close_btn_);
	title_hbox_layout_->addSpacing(12);
	main_vbox_layout_->addSpacing(8);
	main_vbox_layout_->addLayout(title_hbox_layout_);

	//消息内容
	content_layout_ = new QHBoxLayout();
	content_layout_->setContentsMargins(0, 0, 0, 0);
	content_layout_->setSpacing(0);

	content_lab_ = new QLabel(this);
	content_lab_->setFixedSize(250, 20);
	content_lab_->setText(QStringLiteral(""));
	content_lab_->setStyleSheet(s_lab_content_style);
	content_lab_->setAlignment(Qt::AlignLeft);

	content_layout_->addSpacing(20);
	content_layout_->addWidget(content_lab_, 5);
	content_layout_->addSpacing(12);

	main_vbox_layout_->addSpacing(8);
	main_vbox_layout_->addLayout(content_layout_);

	// 相关操作
	operation_hbox_layout_ = new QHBoxLayout();
	operation_hbox_layout_->setContentsMargins(0, 0, 0, 0);
	operation_hbox_layout_->setSpacing(0);

	ok_btn_ = new QPushButton(this);
	ok_btn_->setFixedSize(96, 30);
	ok_btn_->setStyleSheet(s_btn_ok_style);
	ok_btn_->setText(QStringLiteral("确定"));

	cancel_btn_ = new QPushButton(this);
	cancel_btn_->setFixedSize(96, 30);
	cancel_btn_->setStyleSheet(s_btn_ok_style);
	cancel_btn_->setText(QStringLiteral("取消"));

	operation_hbox_layout_->addSpacing(12);

	operation_hbox_layout_->addStretch();
	operation_hbox_layout_->addWidget(ok_btn_);
	operation_hbox_layout_->addStretch();
	operation_hbox_layout_->addWidget(cancel_btn_);
	operation_hbox_layout_->addSpacing(12);

	main_vbox_layout_->addSpacing(18);
	main_vbox_layout_->addLayout(operation_hbox_layout_);
	main_vbox_layout_->addSpacing(20);
}

void PopupWidget::SetData(const QString& title, const QString& content) {
	title_lab_->setText(title);
	content_lab_->setText(content);
	content_lab_->adjustSize();
}

void PopupWidget::InitSigChannel() {
	connect(ok_btn_, &QPushButton::clicked, this, [=]() {
		done(QDialog::Accepted);
	});

	connect(cancel_btn_, &QPushButton::clicked, this, [=]() {
		done(QDialog::Rejected);
	});
}

}