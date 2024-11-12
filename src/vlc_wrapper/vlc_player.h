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

namespace yk {

class VLCPlayer {
public:
	static std::shared_ptr<VLCPlayer> Make(HWND* hwnd) {
		return std::make_shared<VLCPlayer>(hwnd);
	}

	VLCPlayer(HWND* hwnd);
	~VLCPlayer();

	void OpenMediaFile();

private:
	bool Init();

private:
	libvlc_instance_t* libvlc_instance_ = nullptr;
	libvlc_media_player_t* libvlc_media_player_ = nullptr;
	libvlc_media_t* libvlc_media_ = nullptr;
	libvlc_state_t libvlc_state_;

	HWND* hwnd_ = nullptr;
};

}