#include "vlc_player.h"

#include <fstream>
#include <iostream>
#include <string>
#include <mutex>
#include <qstring.h>
#include <qdebug.h>
#include "cpp_base_lib/time_ext.h"
#include "yk_logger.h"
#include "app_messages.h"
#include "context.h"

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

VLCPlayer::VLCPlayer(const std::shared_ptr<Context>& context, HWND hwnd) : context_(context), hwnd_(hwnd) {
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

bool VLCPlayer::OpenMediaFile(const QString& url) {
	// 自定义IO
	//libvlc_media_ = libvlc_media_new_callbacks(
	//	libvlc_instance_, MediaOpen, MediaRead, MediaSeek, MediaClose, 
	//	(void*)"C:\\code\\proj\\starlight_pc_player\\test_video\\1.mp4"
	//);
	libvlc_media_ = nullptr;
	bool is_local_file = false;
	QString media_url = url;
	if (!media_url.contains("://")) {
		is_local_file = true;
#ifdef WIN32
		media_url.replace(QString("/"), QString("\\"));
#endif // WIN32
	}
	qDebug() << "OpenMediaFile is " << media_url;
	media_file_url_ = media_url;
	std::string url_str = media_url.toStdString();
	const char* url_cstr = url_str.c_str();
	if (is_local_file) {
		libvlc_media_ = libvlc_media_new_path(libvlc_instance_, url_cstr);
	}
	else {
		libvlc_media_ = libvlc_media_new_location(libvlc_instance_, url_cstr);
	}
	if (!libvlc_media_) {
		YK_LOGE("libvlc_media_ is nullptr.");
		return false;
	}
	/* Create a media player playing environement */
	libvlc_media_player_ = libvlc_media_player_new_from_media(libvlc_media_);
	if (!libvlc_media_player_) {
		return false;
	}
	AttachEvents();

	libvlc_video_set_mouse_input(libvlc_media_player_, false);

	libvlc_video_set_key_input(libvlc_media_player_, false);

	libvlc_media_parse(libvlc_media_);

	libvlc_media_release(libvlc_media_);

	libvlc_media_ = nullptr;

	if (!hwnd_) {
		return false;
	}

	libvlc_media_player_set_hwnd(libvlc_media_player_, hwnd_);
	// play the media_player
	libvlc_media_player_play(libvlc_media_player_);

	YK_LOGI("libvlc_media_player_get_length start time : {}", TimeExt::GetCurrentTimestamp());

	duration_ = libvlc_media_player_get_length(libvlc_media_player_);

	YK_LOGI("libvlc_media_player_get_length end time : {}, duration_ : {}", TimeExt::GetCurrentTimestamp(), duration_);

	AppGotDurationMsg duration_msg{.duration = duration_ };
	context_->SendAppMessage(duration_msg);
	return true;
}

void VLCPlayer::Stop() {
	if (NULL != libvlc_media_player_) {
		libvlc_media_player_stop(libvlc_media_player_);
		libvlc_media_player_ = NULL;
	}
	duration_ = 0.0f;
}

void VLCPlayer::Pause() {
	if (NULL == libvlc_media_player_) {
		return;
	}
	libvlc_media_player_pause(libvlc_media_player_);
}

void VLCPlayer::Resume() {
	if (NULL == libvlc_media_player_) {
		return;
	}
	libvlc_media_player_set_pause(libvlc_media_player_, false);
}

void VLCPlayer::SetPosition(float position)
{
	if (NULL == libvlc_media_player_) {
		return;
	}
	libvlc_media_player_set_position(libvlc_media_player_, position / duration_);
}


void VLCPlayer::AttachEvents() {
	// 事件列表
	QList<libvlc_event_e> events;
	events 
		<< libvlc_MediaPlayerOpening
		<< libvlc_MediaPlayerBuffering
		<< libvlc_MediaPlayerPlaying
		<< libvlc_MediaPlayerPaused
		<< libvlc_MediaPlayerStopped
		<< libvlc_MediaPlayerEncounteredError
		<< libvlc_MediaPlayerMuted
		<< libvlc_MediaPlayerUnmuted
		<< libvlc_MediaPlayerAudioVolume
		<< libvlc_MediaPlayerLengthChanged
		<< libvlc_MediaPlayerTimeChanged
		<< libvlc_MediaPlayerPositionChanged
		<< libvlc_MediaPlayerEndReached;
	// 订阅事件
	libvlc_event_manager_ = libvlc_media_player_event_manager(libvlc_media_player_);
	for (auto& event : events) {
		libvlc_event_attach(libvlc_event_manager_, event, HandleLibvlcEvents, this);
	}
}

void VLCPlayer::HandleLibvlcEvents(const libvlc_event_t* event, void* user_data) {
	VLCPlayer* player = static_cast<VLCPlayer*>(user_data);
	switch (event->type) {
		// 播放状态改变
	case libvlc_MediaPlayerOpening: {
		qDebug() << "libvlc_MediaPlayerOpening";
		break;
	}
	case libvlc_MediaPlayerBuffering: {
		break;
	}
	case libvlc_MediaPlayerPlaying: {
		AppLibvlcMediaPlayerPlayingMsg msg{};
		player->context_->SendAppMessage(msg);
		break;
	}
	case libvlc_MediaPlayerPaused: {
		AppLibvlcMediaPlayerPausedMsg msg{};
		player->context_->SendAppMessage(msg);
		break;
	}
	case libvlc_MediaPlayerStopped: {
		//当 libvlc_media_player_stop(libvlc_mp); 被调用才会触发该信号
		AppLibvlcMediaPlayerStoppedMsg msg{};
		player->context_->SendAppMessage(msg);
		break;
	}
	case libvlc_MediaPlayerEncounteredError: {
		//player->MediaPlayerEncounteredError();
		QString errmsg = libvlc_errmsg();
		YK_LOGE("media_file_url : {}, libvlc_MediaPlayerEncounteredError : {}", player->media_file_url_.toStdString(), errmsg.toStdString());
		AppLibvlcMediaPlayerEncounteredErrorMsg app_msg{.content = errmsg};
		player->context_->SendAppMessage(app_msg);
		break;
	}
	// 时长改变时获取一次总时长
	case libvlc_MediaPlayerLengthChanged: {
		player->duration_ = libvlc_media_player_get_length(player->libvlc_media_player_);
		AppGotDurationMsg duration_msg{ .duration = player->duration_ };
		player->context_->SendAppMessage(duration_msg);
		break;
	}
	// 播放时间改变
	case libvlc_MediaPlayerTimeChanged: {
		//播放时间是一直在变的
		uint64_t curtime = libvlc_media_player_get_time(player->libvlc_media_player_);
		AppLibvlcMediaPlayerTimeChangedMsg msg{ .current_movie_time = curtime };
		player->context_->SendAppMessage(msg);
		break;
	}
	// 播放位置改变
	case libvlc_MediaPlayerPositionChanged: {
		//播放位置是一直在变的,个人认为等同 libvlc_MediaPlayerTimeChanged
		break;
	}
	case libvlc_MediaPlayerEndReached: {
		//当视频播放结束时触发该信号
		AppLibvlcMediaPlayerEndReachedMsg msg{};
		player->context_->SendAppMessage(msg);
		break;
	}
	default:
		break;
	}
}

}