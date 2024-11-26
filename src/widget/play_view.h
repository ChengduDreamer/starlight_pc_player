#pragma once

#include <qwidget.h>
#include <qstring.h>
namespace yk {
class Context;
class VLCPlayer;

class PlayView : public QWidget {
	Q_OBJECT
public:
	PlayView(const std::shared_ptr<Context>& context, QWidget* parent = nullptr);
	~PlayView();

	bool Play(const QString& url);
Q_SIGNALS:
	void SigGetDuration();
private:
	std::shared_ptr<VLCPlayer> vlc_player_ptr_ = nullptr;
	std::shared_ptr<Context> context_ = nullptr;
};


}