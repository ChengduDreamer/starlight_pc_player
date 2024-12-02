#include "settings.h"
#include <qdir.h>
#include "cpp_base_lib/str_util.h"
#include "cpp_base_lib/path_util.h"
#include "cpp_base_lib/yk_logger.h"

namespace yk {
Settings::Settings() {
	auto vcapdir = GetProgramDataDir(L"\\StarlightPlayer\\CaptureImage");
	video_capture_dir_ = ToUTF8(vcapdir);
	YK_LOGI("video_capture_dir : {}", video_capture_dir_);
	auto qvcapdir = QString::fromStdString(video_capture_dir_);
	qvcapdir = qvcapdir.replace("\\", "/");
	QDir dir{ qvcapdir };
	if (!dir.exists()) {
		dir.mkpath(qvcapdir);
	}
	// to do ÅÐ¶Ï,°ÑÖ®Ç°µÄ½ØÍ¼É¾³ýµô
}

Settings::~Settings() {
	
}
}