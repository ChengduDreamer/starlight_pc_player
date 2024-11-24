#pragma once

#include <qwidget.h>
#include <qstring.h>
namespace yk {

class VLCPlayer;

class PlayView : public QWidget {
	Q_OBJECT
public:
	PlayView(QWidget* parent = nullptr);
	~PlayView();

	void Play(const QString& url);

private:
	std::shared_ptr<VLCPlayer> vlc_player_ptr_ = nullptr;
};


}