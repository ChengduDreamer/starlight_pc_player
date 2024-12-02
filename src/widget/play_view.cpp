#include "play_view.h"
#include <qsizepolicy.h>
#include <qdebug.h>
#include <qpixmap.h>
#include <qdatetime.h>
#include <qfile.h>
#include "vlc_wrapper/vlc_player.h"
#include "context.h"
#include "app_messages.h"
#include "settings.h"
namespace yk {

PlayView::PlayView(const std::shared_ptr<Context>& context, QWidget* parent) : context_(context), QWidget(parent) {
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setStyleSheet("QWidget {background-color: #000000;}");
	HWND render_hwnd = (HWND)(this->winId());
	vlc_player_ptr_ = VLCPlayer::Make(context_, render_hwnd);
	EnableWindow(render_hwnd, FALSE); // 能让QT窗口接受事件
	context_->video_render_widget_ = reinterpret_cast<QWidget*>(this);

	msg_listener_ = context_->CreateMessageListener();
	msg_listener_->Listen<AppCaptureImageMsg>([=, this](const AppCaptureImageMsg& event) {
		context_->PostUITask([=, this]() {
			static int count = -1;
			QString png_name = QString::number(QDateTime::currentMSecsSinceEpoch()) + QString("_") + QString::number(++count) + ".png";
			std::string file_path = context_->GetSettings()->video_capture_dir_ + "\\" + png_name.toStdString();
			AppCaptureImageCompletedMsg msg;
			if (!vlc_player_ptr_->TakeSnapshot(file_path)) {
				msg.success = false;
				context_->SendAppMessage(msg);
				return;
			}
			msg.success = true;
			msg.pixmap = QPixmap(QString::fromStdString(file_path));
			context_->SendAppMessage(msg);
			QFile png_file{ QString::fromStdString(file_path) };
			png_file.remove();
		});
	});

	
}

PlayView::~PlayView() {

}

void PlayView::keyPressEvent(QKeyEvent* event) {
	QWidget::keyPressEvent(event);
}

void PlayView::mouseDoubleClickEvent(QMouseEvent* event) {

	if (event->button() == Qt::LeftButton) {
		AppPlayViewMouseDoubleClickedMsg msg{};
		context_->SendAppMessage(msg);
	}
	QWidget::mouseDoubleClickEvent (event);
}


bool PlayView::Play(const QString& url) {
	return vlc_player_ptr_->OpenMediaFile(url);
}

void PlayView::Stop() {
	vlc_player_ptr_->Stop();
}

void PlayView::Pause() {
	vlc_player_ptr_->Pause();
}

void PlayView::Resume() {
	vlc_player_ptr_->Resume();
}

void PlayView::SeekPos(int ms) {
	vlc_player_ptr_->SetPosition(ms);
}

void PlayView::SetMute() {
	vlc_player_ptr_->SetMute();
}


void PlayView::SetUnmute() {
	vlc_player_ptr_->SetUnmute();
}

void PlayView::SetVolume(int volume) {
	vlc_player_ptr_->SetVolume(volume);
}



}