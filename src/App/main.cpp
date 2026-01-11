#include <iostream>
#include <print>
#include "App/App.hpp"
#include "LowLevelShit/Shader.hpp"

int main()
{
    AppSettings settings;
    settings.AppName = "Game!";
    settings.fps = 144;
    settings.WindowHeight = 800;
    settings.WindowWidth = 600;

    App app(settings);
    app.Run();

    return 0;
}