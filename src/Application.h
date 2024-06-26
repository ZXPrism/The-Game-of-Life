#pragma once

#include <functional>
#include <vector>

#include <SDL.h>

#include "Renderer.h"

class Application
{
public:
    void Init(int windowSideLength);
    void Run(const std::function<void(float)> &callback);
    void Shutdown();

    void ProcessEvents();
    using OnCursorPosFunc = std::function<void(double, double)>;
    using OnKeyFunc = std::function<void(int, int, int, int)>;
    using OnMouseButtonFunc = std::function<void(int, int, int)>;
    using OnWindowSizeFunc = std::function<void(int, int)>;

    void RegisterOnKeyFunc(OnKeyFunc func);
    void RegisterOnMouseButtonFunc(OnMouseButtonFunc func);
    void RegisterOnCursorPosFunc(OnCursorPosFunc func);
    void RegisterOnWindowSizeFunc(OnWindowSizeFunc func);

    void OnKey(int key, int scanCode, int action, int mods);
    void OnMouseButton(int button, int action, int mods);
    void OnCursorPos(double xPos, double yPos);
    void OnWindowSize(int width, int height);

    Renderer &GetRenderer();

private:
    SDL_Window *_pWindow;

    Renderer _renderer;

    bool _AppRunning = true;

    std::vector<OnCursorPosFunc> _OnCursorPosVec;
    std::vector<OnKeyFunc> _OnKeyVec;
    std::vector<OnMouseButtonFunc> _OnMouseButtonVec;
    std::vector<OnWindowSizeFunc> _OnWindowSizeVec;
};
