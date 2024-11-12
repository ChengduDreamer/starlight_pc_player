#pragma once

#include <qwidget.h>
namespace yk {

class VLCPlayer;

class PlayView : public QWidget {
public:
	PlayView(QWidget* parent = nullptr);
	~PlayView();

	void Play();

private:
	std::shared_ptr<VLCPlayer> vlc_player_ptr_ = nullptr;
};


}