#include "vlc_player.h"

#include <fstream>
#include <iostream>
#include <string>

namespace yk {


	struct yk_media_file_t
	{
		
	};

std::ifstream file; // 使用宽字符

int	MediaOpen(void* opaque, void** datap, uint64_t* sizep)
{
	file.open((char*)opaque, std::ios::binary | std::ios::in);
	file.seekg(0, std::ios::end);
	int len = file.tellg();
	file.seekg(0);
	*sizep = len;
	*datap = &file;
	return 0;
}

ssize_t MediaRead(void* opaque, unsigned char* buf, size_t len)
{
	std::ifstream* in = (std::ifstream*)opaque;
	in->read((char*)buf, len);
	auto s = in->gcount();

	std::cout << "len = " << len << std::endl;

	std::cout << "s = " << s << std::endl;

	if (s == 0)
	{
		if (in->eof())
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
	return s;
}
int	MediaSeek(void* opaque, uint64_t offset)
{
	std::ifstream* in = (std::ifstream*)opaque;
	in->clear();
	in->seekg(offset);
	std::cout << "offset = " << offset << std::endl;
	return 0;
}
void MediaClose(void* opaque)
{
	std::ifstream* in = (std::ifstream*)opaque;
	in->close();
	std::cout << "close\n";
}

VLCPlayer::VLCPlayer(HWND* hwnd) : hwnd_(hwnd) {
	Init();
}


VLCPlayer::~VLCPlayer() {
	
}

bool VLCPlayer::Init() {
	const char* vlc_args[] = {
		"--sub-filter=logo",
		"--mouse-hide-timeout=2147483647",
		"--no-xlib",                // 避免使用 Xlib（在某些平台上可能需要）
		"--vout=opengl"           // 指定使用 OpenGL 作为视频输出
	};
	libvlc_instance_ = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
	if (!libvlc_instance_) {
		return false;
	}
	return true;
}

void VLCPlayer::OpenMediaFile() {
	//libvlc_media_ = libvlc_media_new_callbacks(
	//	libvlc_instance_, MediaOpen, MediaRead, MediaSeek, MediaClose, 
	//	(void*)"C:\\code\\proj\\starlight_pc_player\\test_video\\1.mp4"
	//);

	libvlc_media_ = libvlc_media_new_path(libvlc_instance_,"C:\\code\\proj\\starlight_pc_player\\test_video\\1.mp4");

	/* Create a media player playing environement */
	libvlc_media_player_ = libvlc_media_player_new_from_media(libvlc_media_);
	libvlc_video_set_mouse_input(libvlc_media_player_, false);
	libvlc_video_set_key_input(libvlc_media_player_, false);

	libvlc_media_parse(libvlc_media_);

	if (!hwnd_) {
		return;
	}
	//EnableWindow(*hwnd_, FALSE);// 能让qt窗口接收到事件
	libvlc_media_player_set_hwnd(libvlc_media_player_, hwnd_);
	// play the media_player
	libvlc_media_player_play(libvlc_media_player_);
}


}