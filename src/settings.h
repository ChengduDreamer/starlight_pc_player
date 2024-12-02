#pragma once
#include <string>
namespace yk {
class Settings {
public:
    static Settings* Instance() {
        static Settings st;
        return &st;
    }
public:
    std::string video_capture_dir_;
private:
    Settings();
    ~Settings();


};

}