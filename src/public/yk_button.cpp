#include "yk_button.h"

#include <qbrush.h>
#include <qfont.h>


YKButton::YKButton(QWidget* parent) : QPushButton(parent) {

}

void YKButton::Init(const QSize& size, TextInfo text_info, BackgroundInfo background_info, IconInfo icon_info, BorderInfo border_info) {
	setFixedSize(size);
	setAttribute(Qt::WA_StyledBackground);
	setMouseTracking(true);
	
	m_text_info = text_info;
	m_background_info = background_info;
	m_icon_info = icon_info;
	m_border_info = border_info;

	m_pixmap_normal = QPixmap(icon_info.m_icon_normal);
	m_pixmap_hover = QPixmap(icon_info.m_icon_hover);
	m_pixmap_press = QPixmap(icon_info.m_icon_press);
	m_pixmap_disable = QPixmap(icon_info.m_icon_disable);

	if (m_icon_info.m_icon_normal.endsWith(".svg")) {
		m_use_svg = true;
	}
}

void YKButton::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setPen(Qt::NoPen);
	painter.save(); // QPainter::end: Painter ended with 2 saved states  QPainter �� save ������ restore ����Ӧ�ɶԳ��֣���Ҫ��©��
	
	QPen pen;
	pen.setStyle(Qt::SolidLine);
	pen.setWidth(m_border_info.m_border_width);
	
	if (isEnabled()) {
		if (m_pressed) {
			pen.setColor(m_border_info.m_color_press);
			painter.setBrush(QBrush(m_background_info.m_background_color_press));
			
		}
		else {
			if (m_cursor_in) {
				pen.setColor(m_border_info.m_color_hover);
				painter.setBrush(QBrush(m_background_info.m_background_color_hover));
			}
			else {
				pen.setColor(m_border_info.m_color_normal);
				painter.setBrush(QBrush(m_background_info.m_background_color_normal));
			}
		}
	}
	else {
		pen.setColor(m_border_info.m_color_press);
		painter.setBrush(QBrush(m_background_info.m_background_color_press));
	}

	if (m_border_info.m_border_width > 0) {
		painter.setPen(pen);
	}
	painter.drawRoundedRect(this->rect(), m_border_info.m_border_radius, m_border_info.m_border_radius);
	painter.restore();
	painter.save(); // QPainter::restore: Unbalanced save/restore  restore �� save�ǳɶԴ��ڵ�
	if (m_icon_info.m_have_icon) {
		if (isEnabled()) {
			if (m_pressed) {
				if (m_use_svg) {
					m_svg_renderer.load(m_icon_info.m_icon_press);
					m_svg_renderer.render(&painter, QRectF(m_icon_info.m_padding_left, m_icon_info.m_padding_top, 
						m_icon_info.m_icon_size.width(), m_icon_info.m_icon_size.height()));
				}
				else {
					painter.drawPixmap(m_icon_info.m_padding_left, m_icon_info.m_padding_top, m_pixmap_press);
				}
			}
			else {
				if (m_cursor_in) {
					if (m_use_svg) {
						m_svg_renderer.load(m_icon_info.m_icon_hover);
						m_svg_renderer.render(&painter, QRectF(m_icon_info.m_padding_left, m_icon_info.m_padding_top,
							m_icon_info.m_icon_size.width(), m_icon_info.m_icon_size.height()));
					}
					else {
						painter.drawPixmap(m_icon_info.m_padding_left, m_icon_info.m_padding_top, m_pixmap_hover);
					}
				}
				else {
					if (m_use_svg) {
						m_svg_renderer.load(m_icon_info.m_icon_normal);
						m_svg_renderer.render(&painter, QRectF(m_icon_info.m_padding_left, m_icon_info.m_padding_top,
							m_icon_info.m_icon_size.width(), m_icon_info.m_icon_size.height()));
					}
					else {
						painter.drawPixmap(m_icon_info.m_padding_left, m_icon_info.m_padding_top, m_pixmap_normal);
					}
					
				}
			}
		}
		else {
			if (m_use_svg) {
				m_svg_renderer.load(m_icon_info.m_icon_disable);
				m_svg_renderer.render(&painter, QRectF(m_icon_info.m_padding_left, m_icon_info.m_padding_top,
					m_icon_info.m_icon_size.width(), m_icon_info.m_icon_size.height()));
			}
			else {
				painter.drawPixmap(m_icon_info.m_padding_left, m_icon_info.m_padding_top, m_pixmap_disable);
			}
		}
	}

	painter.restore();
	QFont font{ "Microsoft YaHei" };
	font.setPixelSize(m_text_info.m_font_size);
	font.setBold(m_text_info.m_blod);
	QPen font_pen;
	if (isEnabled()) {
		if (m_pressed) {
			font_pen.setColor(m_text_info.m_font_color_press);
		}
		else {
			if (m_cursor_in) {
				font_pen.setColor(m_text_info.m_font_color_hover);
			}
			else {
				font_pen.setColor(m_text_info.m_font_color_normal);
			}
		}
	}
	else {
		font_pen.setColor(m_text_info.m_font_color_disable);
	}
	painter.setPen(font_pen);
	painter.setFont(font);
	painter.drawText(m_text_info.m_padding_left, m_text_info.m_padding_top, m_text_info.m_text); // ע�� �����y����  �� ���ֵײ� ���� ��ť�ϱ߿� ����
}


void YKButton::mousePressEvent(QMouseEvent* event) {
	m_pressed = true;
	repaint();
	QPushButton::mousePressEvent(event);
}

void YKButton::mouseReleaseEvent(QMouseEvent* event) {
	m_pressed = false;
	repaint();
	QPushButton::mouseReleaseEvent(event);
}

void YKButton::enterEvent(QEnterEvent* event) {
	m_cursor_in = true;
	repaint();
	QPushButton::enterEvent(event);
}

void YKButton::leaveEvent(QEvent* event) {
	m_cursor_in = false;
	repaint();
	QPushButton::leaveEvent(event);
}
