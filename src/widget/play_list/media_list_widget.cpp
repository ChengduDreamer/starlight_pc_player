#include "media_list_widget.h"
#include <qapplication.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qdesktopwidget.h>
#include <qurl.h>
#include <qurlquery.h>
#include <qpainter.h>
#include <qboxlayout.h>

static QString kCallBackHomePageButtonStyle = "QPushButton{border:0px groove gray;border-radius:6px;background-color:#4E99EC;padding:2px 4px;border-style: solid;\
		font-family:Microsoft YaHei;font-size:%1px; color:#ffffff;}";

namespace yk {

	MediaItemWidget::MediaItemWidget(int index, QWidget* parent) : index_(index), QWidget(parent) {
		InitView();
	}

	void MediaItemWidget::paintEvent(QPaintEvent* event) {
		QPainter painter(this);
		if (cursor_in_) {
			painter.setBrush(QColor(0x42, 0x42, 0x42));
		}
		else {
			painter.setBrush(QColor(0x21, 0x21, 0x21));
			if (index_ % 2 == 0) {
				painter.setBrush(QColor(0x19, 0x19, 0x19));
			}
		}
		painter.drawRect(0, 0, this->width(), this->height());
	}

	void MediaItemWidget::enterEvent(QEvent* event) {
		cursor_in_ = true;
		repaint();
	}
	void MediaItemWidget::leaveEvent(QEvent* event) {
		cursor_in_ = false;
		repaint();
	}

	void MediaItemWidget::mouseDoubleClickEvent(QMouseEvent* e) {
		// to do play
	}

	void MediaItemWidget::SetData(const QString& name, const QString& url) {
		name_lab_->setText(name);
		url_ = url;
	}

	void MediaItemWidget::InitView() {
		vbox_main_layout_ = new QVBoxLayout(this);
		vbox_main_layout_->setAlignment(Qt::AlignTop);
		vbox_main_layout_->setMargin(0);
		vbox_main_layout_->setSpacing(0);
		setLayout(vbox_main_layout_);
		setAttribute(Qt::WA_StyledBackground);// 解决 背景色只有其中部件生效的问题。
		hbox_layout_ = new QHBoxLayout();
		hbox_layout_->setAlignment(Qt::AlignLeft);
		hbox_layout_->setMargin(0);
		hbox_layout_->setSpacing(0);
		name_lab_ = new QLabel(this);
		//name_lab_->setStyleSheet(kTableRecordStyle);
		name_lab_->setText(QStringLiteral("测试用"));
		name_lab_->setFixedSize(200, 48);
	
	
		del_btn_ = new QPushButton(this);
		del_btn_->setFixedSize(62, 36);
		del_btn_->setText(QStringLiteral("删除"));
	
	}

	//列表
	MediaListWidget::MediaListWidget(QWidget* parent) : ListWidget(parent) {
		InitView();
	}

	void MediaListWidget::InitView() {
		setStyleSheet(R"(
			QListWidget{
				border:none;
				background:transparent;
				}
				QListWidget::itemn
				{
					background: transparent;		
				}
				QListWidget::item:selected	
				{								
						background:transparent;		
				}								
		)");
		QSizePolicy sizePolicy = this->sizePolicy();
		sizePolicy.setVerticalPolicy(QSizePolicy::Expanding);
		sizePolicy.setHorizontalPolicy(QSizePolicy::Expanding);
		setSizePolicy(sizePolicy);
	}


	QListWidgetItem* MediaListWidget::AddItem(MediaItemWidget* pItem) {
		//新建一个item
		QListWidgetItem* item = new QListWidgetItem();
		item->setSizeHint(QSize(50,48));
		this->addItem(item);
		// 新建一个widget
		this->setItemWidget(item, pItem);
		item->setHidden(true);
		return item;
	}
}