#include "vlc_player.h"

#include <fstream>
#include <iostream>
#include <string>
#include <mutex>
#include <qstring.h>
#include <qdebug.h>
#include <qtimer.h>
#include "cpp_base_lib/time_ext.h"
#include "cpp_base_lib/file.h"
#include "cpp_base_lib/data.h"
#include "yk_logger.h"
#include "app_messages.h"
#include "context.h"

namespace yk {

//libvlc_media_new_callbacks

FILE* g_custom_libvlc_log_file_ptr_ = nullptr;

std::shared_ptr<yk::File> local_media_file_ptr_ = nullptr;

std::shared_ptr<yk::Data> media_data_ptr_ = nullptr;

struct CustomFileContext {
	
};

#if 0
enum libvlc_log_level
{
	LIBVLC_DEBUG = 0,   /**< Debug message */
	LIBVLC_NOTICE = 2,  /**< Important informational message */
	LIBVLC_WARNING = 3, /**< Warning (potential error) message */
	LIBVLC_ERROR = 4    /**< Error message */
};
#endif

void CustomLibVlcLogCallback(void* data, int level, const libvlc_log_t* ctx, const char* fmt, va_list args)
{
	char buffer[2048] = {0, }; // 存储格式化后的日志信息
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	// 输出到控制台
	std::cout << "[CustomLibVlcLogCallback:] " << buffer << std::endl; // 乱码
	std::string log_content = buffer;
	log_content += "\n";
	//std::cout << "log_content size: " << log_content.size() << std::endl;
	// 额外写入到日志文件
	auto file_ptr = static_cast<FILE*>(data);
	if (file_ptr) {
		size_t written = fwrite(log_content.data(), 1, log_content.size(), file_ptr);
		if (written != log_content.size()) {
			std::cerr << "Write error: " << strerror(errno) << std::endl;
		}
		fflush(file_ptr); // 关键！
	}
}


std::ifstream file; // 使用宽字符

FILE* g_libvlc_file_ = nullptr;

int	OpenMedia(void* opaque, void** datap, uint64_t* sizep) {

	std::cout << "---------------------------------------------------------------------OpenMedia" << std::endl;
	auto file_size_res = local_media_file_ptr_->Size();
	if (file_size_res.has_value()) {
		*sizep = file_size_res.value();
		return 0;
	}
	return -1;
}

ssize_t ReadMedia(void* opaque, unsigned char* buf, size_t len) {
	std::cout << "---------------------------------------------------------------------ReadMedia" << std::endl;
	auto data_ptr = local_media_file_ptr_->Read(len);
	if (!data_ptr) {
		if (local_media_file_ptr_->IsEnd()) {
			return 0;
		}
		return -1;
	}
	memcpy(buf, data_ptr->CStr(), data_ptr->Size());
	return data_ptr->Size();
}
int	SeekMedia(void* opaque, uint64_t offset) {
	std::cout << "---------------------------------------------------------------------SeekMedia" << std::endl;
	if (local_media_file_ptr_->Seek(offset)) {
		return 0;
	}
	return -1;
}

void CloseMedia(void* opaque) {
	std::cout << "---------------------------------------------------------------------CloseMedia" << std::endl;
	local_media_file_ptr_->Close();
}

VLCPlayer::VLCPlayer(const std::shared_ptr<Context>& context, HWND hwnd) : context_(context), hwnd_(hwnd) {
	QObject();
	msg_listener_ = context_->CreateMessageListener();
	Init();
	RegisterEvents();
}

VLCPlayer::~VLCPlayer() {
	if (g_custom_libvlc_log_file_ptr_) {
		fclose(g_custom_libvlc_log_file_ptr_);
	}
}

bool VLCPlayer::Init() {
	const char* vlc_args[] = {
		"--sub-filter=logo",
		"--no-xlib",                // 避免使用 Xlib（在某些平台上可能需要）
		"--vout=opengl"           // 指定使用 OpenGL 作为视频输出
	};
	libvlc_instance_ = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
	if (!libvlc_instance_) {
		return false;
	}

	g_custom_libvlc_log_file_ptr_ = fopen(".\\yk_libvlc_log.txt", "ab");
	// 设置日志回调
	if (g_custom_libvlc_log_file_ptr_) {
		libvlc_log_set(libvlc_instance_, CustomLibVlcLogCallback, static_cast<void*>(g_custom_libvlc_log_file_ptr_));
	}
	return true;
}

// 640 368

void set_marquee(libvlc_media_player_t* player, const char* text, int position) {
	libvlc_video_set_marquee_int(player, libvlc_marquee_Enable, 1);
	libvlc_video_set_marquee_int(player, libvlc_marquee_Color, 0xFF0000); // 白色

	std::string spaces(28 * 4, ' '); // 生成25个空格
	QString spaces_str = QString::fromStdString(spaces);
	//QString text_str = QStringLiteral("水%1试 \n\n\n\n\n\n\n\n\n\n\n\n\n\n测").arg(spaces_str);

	QString text_str = QStringLiteral("水\n\t\n\t\n\t\n\t\n\t\n\t\n\t\n\t\n\t\n\t\n\t\n\t\n\t\n\t\n\t\n\t测");

	//QString text_str = QStringLiteral("<span style='line-height: 2em;'>水<br><br><br><br><br><br><br><br><br><br><br><br>测</span>");

	std::string text_str_std = text_str.toStdString();

	libvlc_video_set_marquee_string(player, libvlc_marquee_Text, text_str_std.c_str());
	libvlc_video_set_marquee_int(player, libvlc_marquee_Opacity, 255);
	libvlc_video_set_marquee_int(player, libvlc_marquee_Size, 20);
	//libvlc_video_set_marquee_int(player, libvlc_marquee_Position, 0);
	libvlc_video_set_marquee_int(player, libvlc_marquee_Timeout, 10); // 持续显示
	libvlc_video_set_marquee_int(player, libvlc_marquee_Refresh, 100); // 不刷新
	
	 // 新增：强制刷新显示
	/*libvlc_video_set_marquee_int(player, libvlc_marquee_X, 10);
	libvlc_video_set_marquee_int(player, libvlc_marquee_Y, 10);*/

	// 设置水印距离左上角的偏移量（单位：像素）
	static int x = 20;
	libvlc_video_set_marquee_int(player, libvlc_marquee_X, 0);  // 水平右移20px
	libvlc_video_set_marquee_int(player, libvlc_marquee_Y, 0);  // 垂直下移30px
}

bool VLCPlayer::OpenMediaFile(const QString& url) {
	
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

	local_media_file_ptr_ = yk::File::OpenForReadB(url_str);

	if (use_custom_io_) {
		// 自定义IO
		if (is_local_file) {
			libvlc_media_ = libvlc_media_new_callbacks(
				libvlc_instance_, OpenMedia, ReadMedia, SeekMedia, CloseMedia,
				nullptr
			);
		}
		else {
			


		}
		
	}
	else {
		if (is_local_file) {
			libvlc_media_ = libvlc_media_new_path(libvlc_instance_, url_cstr);
		}
		else {
			libvlc_media_ = libvlc_media_new_location(libvlc_instance_, url_cstr);
		}
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

	AppPlayMediaMsg play_msg{.url = media_url };
	context_->SendAppMessage(play_msg);
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

void VLCPlayer::SetVolume(int volume) {
	if (NULL == libvlc_media_player_) {
		return;
	}
	libvlc_audio_set_volume(libvlc_media_player_, volume);
}

void VLCPlayer::SetMute() {
	if (NULL == libvlc_media_player_) {
		return;
	}
	libvlc_audio_set_mute(libvlc_media_player_, true);
}

void VLCPlayer::SetUnmute() {
	if (NULL == libvlc_media_player_) {
		return;
	}
	libvlc_audio_set_mute(libvlc_media_player_, false);
}

bool VLCPlayer::TakeSnapshot(const std::string& file_path) {
	if (NULL == libvlc_media_player_) {
		return false;
	}

	YK_LOGI("TakeSnapshot file_path : {}", file_path);

	return 0 == libvlc_video_take_snapshot(libvlc_media_player_, 0, file_path.c_str(), 0, 0);
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

void VLCPlayer::RegisterEvents() {
	msg_listener_->Listen<AppPlayViewKeySpaceMsg>([=, this](const AppPlayViewKeySpaceMsg& event) {
		context_->PostUITask([=, this]() {
			if (NULL == libvlc_media_player_) {
				return;
			}
			libvlc_state_t state = libvlc_media_player_get_state(libvlc_media_player_);
			switch (state) {
			case libvlc_Playing:
				std::cout << "Player is currently playing." << std::endl;
				Pause();
				break;
			case libvlc_Paused:
				std::cout << "Player is currently paused." << std::endl;
				Resume();
				break;
			case libvlc_Stopped:
				std::cout << "Player is stopped." << std::endl;
				break;
			case libvlc_Ended:
				std::cout << "Playback has ended." << std::endl;
				break;
			case libvlc_Error:
				std::cout << "An error occurred with the player." << std::endl;
				break;
			default:
				std::cout << "Player is in an unknown state." << std::endl;
				break;
			}
		});
	});
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
		qDebug() << "libvlc_MediaPlayerBuffering";
		break;
	}
	case libvlc_MediaPlayerPlaying: {
		qDebug() << "libvlc_MediaPlayerPlaying";
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
	case libvlc_MediaPlayerMuted: {
		AppLibvlcMediaPlayerMutedMsg msg{};
		player->context_->SendAppMessage(msg);
		break;
	}
	case libvlc_MediaPlayerUnmuted: {
		AppLibvlcMediaPlayerUnmutedMsg msg{};
		player->context_->SendAppMessage(msg);
		break;
	}
	default:
		break;
	}
}

void VLCPlayer::SetUseCustomIO(bool use) {
	use_custom_io_ = use;
}

}