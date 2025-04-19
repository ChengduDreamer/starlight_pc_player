#pragma once

#include <qlistwidget.h>

namespace yk {

	class ListWidget : public QListWidget {
		Q_OBJECT
	public:
		ListWidget(QWidget* parent = nullptr);
		~ListWidget() = default;
	};

}