#pragma once

#ifdef _WIN32
#include <basetsd.h>
#include <Windows.h>
typedef SSIZE_T ssize_t;
#endif
extern "C" {
#include <vlc/vlc.h>
}
#include <memory>
#include <functional>
#include <qstring.h>
#include <string>
namespace yk {

class Context;
class MessageListener;

class VLCPlayer {
public:
	static std::shared_ptr<VLCPlayer> Make(const std::shared_ptr<Context>& context, HWND hwnd) {
		return std::make_shared<VLCPlayer>(context, hwnd);
	}

	VLCPlayer(const std::shared_ptr<Context>& context, HWND hwnd);
	~VLCPlayer();
	void RegisterEvents();
	bool OpenMediaFile(const QString& url);
	void Stop();
	void Pause();
	void Resume();
	void SetVolume(int volume);
	void SetPosition(float position);
	void SetMute();
	void SetUnmute();
	bool TakeSnapshot(const std::string& file_path);

private:
	bool Init();
	void AttachEvents();
	static void HandleLibvlcEvents(const libvlc_event_t* event, void* user_data);
private:
	libvlc_instance_t* libvlc_instance_ = nullptr;
	libvlc_media_player_t* libvlc_media_player_ = nullptr;
	libvlc_media_t* libvlc_media_ = nullptr;
	libvlc_state_t libvlc_state_;
	libvlc_event_manager_t* libvlc_event_manager_ = nullptr;

	QString media_file_url_;
	HWND hwnd_ = NULL;
	float duration_ = 0.0f;

	std::shared_ptr<Context> context_ = nullptr;
	std::shared_ptr<MessageListener> msg_listener_ = nullptr;
};

}