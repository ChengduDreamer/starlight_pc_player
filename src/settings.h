#pragma once

namespace yk {
class Settings {
public:
    static Settings* Instance() {
        static Settings st;
        return &st;
    }
private:
    Settings();
    ~Settings();
};

}