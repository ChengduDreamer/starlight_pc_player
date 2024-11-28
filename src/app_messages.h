#pragma once
#include <qstring.h>
namespace yk {
	
	// 1000ms timer
	class AppTimer1000Msg {

	};

	class AppOpenUrlMsg {
	public:
		QString url;
	};

	class AppLibvlcMediaPlayerEncounteredErrorMsg {
	public:
		QString content;
	};

	class AppStopPlayMsg {
	public:
		
	};

	class AppPausePlayMsg {
	public:

	};

	class AppResumePlayMsg {
	public:

	};

	class AppSeekPosMsg {
	public:
		int pos = 0;
	};

	class AppSetVolumeMsg {
	public:
		int volume = 0; // [0, 100]
	};

	class AppSetMuteMsg {
	public:
		
	};

	class AppSetUnmuteMsg {
	public:

	};

	class AppGotDurationMsg {
	public:
		float duration = 0.0f; // ms
	};

	// 当媒体播放器开始打开媒体时触发。
	class AppLibvlcMediaPlayerOpeningMsg {
	public:

	};

	// 当媒体播放器需要缓冲数据时触发。
	class AppLibvlMediaPlayerBufferingMsg {
	public:

	};

	// 当媒体播放器开始播放媒体时触发。
	class AppLibvlcMediaPlayerPlayingMsg {
	public:

	};

	class AppLibvlcMediaPlayerPausedMsg {
	public:

	};

	class AppLibvlcMediaPlayerStoppedMsg {
	public:

	};

	class AppLibvlcMediaPlayerEndReachedMsg {
	public:

	};

	class AppLibvlcMediaPlayerTimeChangedMsg {
	public:
		uint64_t current_movie_time = 0; //ms
	};

	class AppLibvlcMediaPlayerMutedMsg {
	public:
		
	};

	class AppLibvlcMediaPlayerUnmutedMsg {
	public:
		
	};
}