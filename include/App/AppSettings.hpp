#pragma once
#include <string>

struct AppSettings {
    std::string AppName = "App";
    int WindowHeight, WindowWidth;
    int fps = 60;
};