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

	class AppGotDurationMsg {
	public:
		float duration = 0.0f; // ms
	};

	// ��ý�岥������ʼ��ý��ʱ������
	class AppLibvlcMediaPlayerOpeningMsg {
	public:

	};

	// ��ý�岥������Ҫ��������ʱ������
	class AppLibvlMediaPlayerBufferingMsg {
	public:

	};

	// ��ý�岥������ʼ����ý��ʱ������
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
}