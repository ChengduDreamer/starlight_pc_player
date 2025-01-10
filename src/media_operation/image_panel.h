#pragma once
#include <memory>
#include <qwidget.h>

class DrawBoardWidget;

namespace yk {
//class ImageToolbar;
//class ImageView;
//class ImageContainer;
class Context;
class MessageListener;

class ImagePanel : public QWidget {
public:
	ImagePanel(const std::shared_ptr<Context>& context, QWidget* parent = nullptr);
	~ImagePanel();
private:
	void InitView();
	void RegisterEvents();
private:
	/*ImageContainer* image_container_ = nullptr;
	ImageToolbar* toolbar_ = nullptr;*/

	DrawBoardWidget* draw_board_widget_ = nullptr;
private:
	std::shared_ptr<Context> context_ = nullptr;
	std::shared_ptr<MessageListener> msg_listener_ = nullptr;
};

}