#pragma once

#include <qwidget.h>
#include <qstring.h>
#include <qevent.h>
namespace yk {
class Context;
class VLCPlayer;

class PlayView : public QWidget {
	Q_OBJECT
public:
	PlayView(const std::shared_ptr<Context>& context, QWidget* parent = nullptr);
	~PlayView();

	void mouseDoubleClickEvent(QMouseEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;

	bool Play(const QString& url);
	void Stop();
	void Pause();
	void Resume();
	void SeekPos(int ms);
	void SetMute();
	void SetUnmute();
	void SetVolume(int volume);
Q_SIGNALS:
	void SigGetDuration();
private:
	std::shared_ptr<VLCPlayer> vlc_player_ptr_ = nullptr;
	std::shared_ptr<Context> context_ = nullptr;
};


}