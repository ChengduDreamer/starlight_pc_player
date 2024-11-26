#include "play_view.h"
#include <qsizepolicy.h>
#include "vlc_wrapper/vlc_player.h"
#include "context.h"
#include "app_messages.h"
namespace yk {

PlayView::PlayView(const std::shared_ptr<Context>& context, QWidget* parent) : context_(context), QWidget(parent) {
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setStyleSheet("QWidget {background-color: #000000;}");
	HWND render_hwnd = (HWND)(this->winId());
	vlc_player_ptr_ = VLCPlayer::Make(render_hwnd);
	EnableWindow(render_hwnd, FALSE); // 能让QT窗口接受事件
}

PlayView::~PlayView() {

}

bool PlayView::Play(const QString& url) {
	return vlc_player_ptr_->OpenMediaFile(url);
}

}