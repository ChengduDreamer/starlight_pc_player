// 加入窗口 一个单独的窗口
#pragma once
#include "list_widget.h"

class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QLabel;


namespace yk {
	//单项
	class MediaItemWidget : public QWidget {
		Q_OBJECT
	public:
		MediaItemWidget(int index, QWidget* parent = nullptr);
		~MediaItemWidget() = default;

		void InitView();
		void SetData(const QString& name, const QString& url);
		

		void paintEvent(QPaintEvent* event) override;
		void enterEvent(QEvent* event) override;
		void leaveEvent(QEvent* event) override;
		void mouseDoubleClickEvent(QMouseEvent* e) override;

		QVBoxLayout* vbox_main_layout_ = nullptr;
		QHBoxLayout* hbox_layout_ = nullptr;

		QLabel*  name_lab_ = nullptr;
		QPushButton* del_btn_ = nullptr;
	private:
		int index_ = 0;
		QString url_;
		bool cursor_in_ = false;
	
	};

	class MediaListWidget : public ListWidget {
		Q_OBJECT
	public:
		MediaListWidget(QWidget* parent = nullptr);
		~MediaListWidget() = default;

		void InitView();
		QListWidgetItem* AddItem(MediaItemWidget* item);
	};

	

	
}