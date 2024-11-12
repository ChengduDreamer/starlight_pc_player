#include <vlc/vlc.h>
#include <iostream>
#include <thread>
#include <stddef.h>

int main() {
    // 初始化libVLC实例
    const char* vlc_args[] = {
        "--no-xlib",  // 不启用X11视频输出（在某些无窗口系统上运行时使用）
    };

    libvlc_instance_t* vlcInstance = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    if (!vlcInstance) {
        std::cerr << "无法初始化libVLC实例！" << std::endl;
        return -1;
    }

    // 设置要播放的视频文件路径
    const char* videoPath = "C:\\code\\proj\\starlight_pc_player\\test_video\\1.mp4";
    libvlc_media_t* media = libvlc_media_new_path(vlcInstance, videoPath);
    if (!media) {
        std::cerr << "无法加载媒体文件！" << std::endl;
        libvlc_release(vlcInstance);
        return -1;
    }

    // 创建媒体播放器
    libvlc_media_player_t* mediaPlayer = libvlc_media_player_new_from_media(media);
    libvlc_media_release(media);  // 媒体加载完后可以释放

    if (!mediaPlayer) {
        std::cerr << "无法创建媒体播放器！" << std::endl;
        libvlc_release(vlcInstance);
        return -1;
    }

    // 开始播放
    libvlc_media_player_play(mediaPlayer);
    std::cout << "正在播放视频..." << std::endl;

    // 等待播放一段时间（此处等待10秒，实际可以根据需要修改）
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // 停止播放并清理
    libvlc_media_player_stop(mediaPlayer);
    libvlc_media_player_release(mediaPlayer);
    libvlc_release(vlcInstance);

    std::cout << "播放结束。" << std::endl;
    return 0;
}

