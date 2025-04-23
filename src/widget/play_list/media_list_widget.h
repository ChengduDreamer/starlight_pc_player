#pragma once
#include "list_widget.h"
#include <memory>
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QLabel;


namespace yk {
	class Context;
	class MessageListener;
	class MediaRecord;
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
		QString name_;
		bool cursor_in_ = false;
	
	};

	//to do: 如果是网络地址，判断能否获取到网络文件名，如果获取不到就直接用最后一个/后面的url部分作为name。

	class MediaListWidget : public ListWidget {
		Q_OBJECT
	public:
		MediaListWidget(const std::shared_ptr<Context>& context, QWidget* parent = nullptr);
		~MediaListWidget() = default;
		QListWidgetItem* AddItem(MediaItemWidget* item);
		void AddMediRecord(std::shared_ptr<MediaRecord> record_ptr);
	private:
		void InitView();
		void RegisterEvents();
	private:
		std::shared_ptr<Context> context_ = nullptr;
		std::shared_ptr<MessageListener> msg_listener_ = nullptr;
	};

	

	
}