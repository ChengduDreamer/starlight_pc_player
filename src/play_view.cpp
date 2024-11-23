#include "play_view.h"
#include <qsizepolicy.h>
#include "vlc_wrapper/vlc_player.h"
namespace yk {

PlayView::PlayView(QWidget* parent) : QWidget(parent) {
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setStyleSheet("QWidget {background-color: #000000;}");
	vlc_player_ptr_ = VLCPlayer::Make((HWND*)this->winId());
}


PlayView::~PlayView() {

}

void PlayView::Play() {
	vlc_player_ptr_->OpenMediaFile();
}

}