#include "play_before_widget.h"
#include <qboxlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qfiledialog.h>
#include <qdebug.h>
#include "public/yk_label.h"
#include "public/yk_button.h"
#include "public/yk_line_edit.h"
#include "app_messages.h"
#include "context.h"

namespace yk {

PlayBeforeWidget::PlayBeforeWidget(const std::shared_ptr<Context>& context, QWidget* parent) : QDragWidget(context, parent) {
	InitView();
	InitSigChannel();
}

void PlayBeforeWidget::InitView() {
	setAttribute(Qt::WA_StyledBackground);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setStyleSheet("QWidget {background-color:#000000;}");
	auto main_vbox_layout = new QVBoxLayout(this);
	main_vbox_layout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
	main_vbox_layout->setSpacing(0);
	main_vbox_layout->setContentsMargins(0,0,0,0);

	//player name
	auto player_name_lab = new YKLabel(this);
	YKLabel::BorderInfo lab_border_info;
	YKLabel::TextInfo lab_text_info;
	lab_border_info.m_width = 0;
	lab_border_info.m_color = QColor(0xff, 0xff, 0xff);
	lab_text_info.m_blod = true;
	lab_text_info.m_color = QColor(0xaa, 0xaa, 0xaa);
	lab_text_info.m_padding_left = 0;
	lab_text_info.m_padding_top = 40;
	lab_text_info.m_text = QStringLiteral("星光播放器");
	lab_text_info.m_size = 40;

	auto logo_font = player_name_lab->GetFont(lab_text_info);
	QFontMetrics fm(logo_font); //计算字体宽度
	auto rect = fm.boundingRect(lab_text_info.m_text);
	qDebug() << "rect = " << rect << ", heigth = " << rect.height();
	player_name_lab->Init(QSize(rect.width(), rect.height()), lab_border_info, lab_text_info, QColor(0x00, 0x00, 0x00, 0x00));

	main_vbox_layout->addStretch(1);
	main_vbox_layout->addWidget(player_name_lab);
	main_vbox_layout->setAlignment(player_name_lab, Qt::AlignHCenter);

	//open local file btn
	open_file_btn_ = new YKButton();
	{
		YKButton::BorderInfo btn_border_info;
		YKButton::TextInfo btn_text_info;
		YKButton::IconInfo btn_icon_info;
		YKButton::BackgroundInfo btn_bk_info;
		btn_border_info.m_border_radius = 4;
		btn_border_info.m_border_width = 0;
		btn_text_info.m_text = QStringLiteral("打开文件");
		btn_text_info.m_font_size = 24;
		btn_text_info.m_blod = false;
		btn_text_info.m_font_color_normal = QColor(0xff, 0xff, 0xff);
		btn_text_info.m_font_color_hover = QColor(0x33, 0x99, 0xff);
		btn_text_info.m_font_color_press = QColor(0x44, 0xaa, 0xff);
		btn_text_info.m_padding_left = 86;
		btn_text_info.m_padding_top = 40;
		btn_icon_info.m_have_icon = true;
		btn_icon_info.m_padding_left = 26;
		btn_icon_info.m_padding_top = 8;
		btn_icon_info.m_icon_normal = ":/resource/play_before/open_file_normal.svg";
		btn_icon_info.m_icon_hover = ":/resource/play_before/open_file_hover.svg";
		btn_icon_info.m_icon_press = ":/resource/play_before/open_file_press.svg";
		btn_icon_info.m_icon_size = QSize(48, 48);
		btn_bk_info.m_background_color_normal = QColor(0x33, 0x33, 0x33);
		btn_bk_info.m_background_color_hover = QColor(0x66, 0x55, 0x60);
		btn_bk_info.m_background_color_press = QColor(0x88, 0x78, 0x80);
		open_file_btn_->Init(QSize(208, 60), btn_text_info, btn_bk_info, btn_icon_info, btn_border_info);
	}
	main_vbox_layout->addStretch(1);
	main_vbox_layout->addWidget(open_file_btn_);
	main_vbox_layout->setAlignment(open_file_btn_, Qt::AlignHCenter);
	


	// url edit
	auto op_url_hbox_layout = new QHBoxLayout();
	op_url_hbox_layout->setSpacing(0);
	op_url_hbox_layout->setContentsMargins(0,0,0,0);
	url_edit_ = new YKLineEdit(this);
	YKLineEdit::BorderInfo edit_border_info;
	YKLineEdit::IconInfo edit_icon_info;
	YKLineEdit::TextInfo edit_text_info;
	edit_border_info.m_border_width = 2;
	edit_border_info.m_border_radius = 0;
	edit_border_info.m_focus_color = QColor(0x44, 0xaa, 0xff);
	edit_border_info.m_hover_color = QColor(0x66, 0x55, 0x60);
	edit_border_info.m_normal_color = QColor(0x00, 0x00, 0x00, 0x00);
	edit_icon_info.m_have_left_icon = false;
	edit_icon_info.m_have_right_icon = false;
	edit_text_info.m_font_size = 14;
	edit_text_info.m_font_color = "#888888";
	edit_text_info.m_padding_left = 10;
	edit_text_info.m_padding_right = 10;

	url_edit_->Init(QSize(550, 52), QStringLiteral("输入资源链接地址，可以立即播放"),
		edit_border_info, edit_icon_info, edit_text_info, QColor(0x6, 0x2f, 0x53));

	open_url_btn_ = new YKButton(this);
	{
		YKButton::BorderInfo btn_border_info;
		YKButton::TextInfo btn_text_info;
		YKButton::IconInfo btn_icon_info;
		YKButton::BackgroundInfo btn_bk_info;

		btn_bk_info.m_background_color_normal = QColor(0x22, 0x46, 0x6e);
		btn_bk_info.m_background_color_hover = QColor(0x32, 0x56, 0x7e);
		btn_bk_info.m_background_color_press = QColor(0x42, 0x66, 0x8e);
		btn_text_info.m_blod = false;
		btn_text_info.m_font_color_normal = QColor(0xff, 0xff, 0xff);
		btn_text_info.m_font_color_hover = QColor(0xff, 0xff, 0xff);
		btn_text_info.m_font_color_press = QColor(0xff, 0xff, 0xff);
		btn_text_info.m_text = QStringLiteral("立即播放");
		btn_text_info.m_font_size = 18;
		btn_text_info.m_padding_left = 24;
		btn_text_info.m_padding_top = 32;
		btn_border_info.m_border_radius = 0;
		btn_border_info.m_border_width = 0;
		btn_icon_info.m_have_icon = false;
		open_url_btn_->Init(QSize(110, 52), btn_text_info, btn_bk_info, btn_icon_info,
			btn_border_info);
	}

	op_url_hbox_layout->addStretch(1);
	op_url_hbox_layout->addSpacing(30);
	op_url_hbox_layout->addWidget(url_edit_);
	op_url_hbox_layout->addWidget(open_url_btn_);
	op_url_hbox_layout->addSpacing(30);
	op_url_hbox_layout->addStretch(1);

	main_vbox_layout->addStretch(1);
	main_vbox_layout->addLayout(op_url_hbox_layout);
	main_vbox_layout->setAlignment(op_url_hbox_layout, Qt::AlignHCenter);
	main_vbox_layout->addStretch(1);
}

void PlayBeforeWidget::InitSigChannel() {
	connect(open_file_btn_, &QPushButton::clicked, this, [=]() {
		QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("选择视频文件"), "C:\\code\\proj\\starlight_pc_player\\test_video", "ALL(*.*)");
		qDebug() << "fileName = " << fileName;
		AppOpenUrlMsg msg{ .url = fileName };
		context_->SendAppMessage(msg);
	});
}

PlayBeforeWidget::~PlayBeforeWidget() {

}

}